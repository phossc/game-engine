#include "engine/ecs/ecs.hpp"

#include "engine/ecs/component_types.hpp"

#include "catch/catch.hpp"

#include <functional>

using engine::Uuid;
using engine::ecs::Behavior_component;
using engine::ecs::Data_component;
using engine::ecs::Ecs;

namespace {

struct Transform_component : Data_component<Transform_component> {
    [[maybe_unused]] static constexpr Uuid uuid{"8a8fe1d9-7204-495b-ba00-d499825af964"};
};

struct Mesh_component : Data_component<Mesh_component, Transform_component> {
    [[maybe_unused]] static constexpr Uuid uuid{"53b49809-eb86-43c5-895d-d24f74849cf3"};
};

struct Player_component
    : Behavior_component<Player_component, Mesh_component, Transform_component> {
    [[maybe_unused]] static constexpr Uuid uuid{"15ccd6bf-cbb8-4ace-8fbb-7ee93fd3e050"};
};

struct Activation_test_component : Behavior_component<Activation_test_component, Player_component> {
    static constexpr Uuid uuid{"6c8c7d79-3afe-41ff-8a7b-e540172b0735"};

    Activation_test_component(std::vector<Uuid>& activated, std::vector<Uuid>& deactivated)
        : activated_{std::ref(activated)}, deactivated_{std::ref(deactivated)} {}

    void activate() override { activated_.get().push_back(get_uuid()); }
    void deactivate() override { deactivated_.get().push_back(get_uuid()); }

    std::reference_wrapper<std::vector<Uuid>> activated_;
    std::reference_wrapper<std::vector<Uuid>> deactivated_;
};

struct Dependent_activation_test_component
    : Behavior_component<Dependent_activation_test_component, Activation_test_component> {
    static constexpr Uuid uuid{"96178390-d747-4810-b018-8a32d47ff0bb"};

    void activate() override {
        get_dependency<Activation_test_component>().activated_.get().push_back(get_uuid());
    }

    void deactivate() override {
        get_dependency<Activation_test_component>().deactivated_.get().push_back(get_uuid());
    }
};

struct Circular_dependency_one
    : Data_component<Circular_dependency_one, struct Circular_dependency_two> {
    [[maybe_unused]] static constexpr Uuid uuid{"c0379bfa-241e-463d-9632-c00a5612660c"};
};

struct Circular_dependency_two
    : Data_component<Circular_dependency_two, Player_component, Circular_dependency_one> {
    [[maybe_unused]] static constexpr Uuid uuid{"a23bd605-445c-44b4-8bd2-31ee41403b8d"};
};

struct Unregistered_component : Data_component<Unregistered_component> {
    [[maybe_unused]] static constexpr Uuid uuid{"63ec9adf-4c0d-460d-817a-421d7d2bf0fb"};
};

} // namespace

TEST_CASE("ECS component registration", "[ecs]") {
    SECTION("Valid dependency graph") {
        Ecs ecs;
        REQUIRE_NOTHROW(ecs.register_components([](const Ecs::Component_registrar& registrar) {
            registrar.register_component<Transform_component>();
            registrar.register_component<Mesh_component>();
            registrar.register_component<Player_component>();
        }));
    }

    SECTION("Circular dependencies") {
        Ecs ecs;
        REQUIRE_THROWS(ecs.register_components([](const Ecs::Component_registrar& registrar) {
            registrar.register_component<Transform_component>();
            registrar.register_component<Mesh_component>();
            registrar.register_component<Player_component>();
            registrar.register_component<Circular_dependency_one>();
            registrar.register_component<Circular_dependency_two>();
        }));
    }

    SECTION("Missing dependency") {
        Ecs ecs;
        REQUIRE_THROWS(ecs.register_components([](const Ecs::Component_registrar& registrar) {
            registrar.register_component<Transform_component>();
            registrar.register_component<Player_component>();
        }));
    }
}

TEST_CASE("ECS entity creation", "[ecs]") {
    Ecs ecs;
    ecs.register_components([](const Ecs::Component_registrar& registrar) {
        registrar.register_component<Transform_component>();
        registrar.register_component<Mesh_component>();
        registrar.register_component<Player_component>();
    });

    SECTION("Valid creation") {
        ecs.entity().with<Transform_component>().build();
        ecs.entity().with<Transform_component>().with<Mesh_component>().build();
        ecs.entity().with<Mesh_component>().with<Transform_component>().build();
        ecs.entity()
                .with<Player_component>()
                .with<Transform_component>()
                .with<Mesh_component>()
                .build();
        REQUIRE_NOTHROW(ecs.lifecycle_update());
    }

    SECTION("Missing dependency") {
        ecs.entity().with<Mesh_component>().build();
        REQUIRE_THROWS(ecs.lifecycle_update());
    }

    SECTION("Creation of unregistered component") {
        REQUIRE_THROWS(ecs.entity().with<Unregistered_component>().build());
    }

    SECTION("Duplicate components") {
        ecs.entity().with<Transform_component>().with<Transform_component>().build();
        REQUIRE_THROWS(ecs.lifecycle_update());
    }
}

TEST_CASE("ECS entity activation/deactivation", "[ecs]") {
    Ecs ecs;
    ecs.register_components([](const Ecs::Component_registrar& registrar) {
        registrar.register_component<Transform_component>();
        registrar.register_component<Mesh_component>();
        registrar.register_component<Player_component>();
        registrar.register_component<Activation_test_component>();
        registrar.register_component<Dependent_activation_test_component>();
    });

    SECTION("Activation and deactivation") {
        std::vector<Uuid> activated_uuids;
        std::vector<Uuid> deactivated_uuids;

        auto entity_id =
                ecs.entity()
                        .with<Activation_test_component>(activated_uuids, deactivated_uuids)
                        .with<Dependent_activation_test_component>()
                        .with<Player_component>()
                        .with<Transform_component>()
                        .with<Mesh_component>()
                        .build();

        REQUIRE(activated_uuids.empty());
        REQUIRE(deactivated_uuids.empty());

        ecs.lifecycle_update();
        REQUIRE(activated_uuids.size() == 2);
        REQUIRE(activated_uuids[0] == Activation_test_component::uuid);
        REQUIRE(activated_uuids[1] == Dependent_activation_test_component::uuid);
        REQUIRE(deactivated_uuids.empty());

        ecs.delete_entity(entity_id);
        REQUIRE(activated_uuids.size() == 2);
        REQUIRE(deactivated_uuids.empty());

        ecs.lifecycle_update();
        REQUIRE(activated_uuids.size() == 2);
        REQUIRE(deactivated_uuids.size() == 2);
        REQUIRE(deactivated_uuids[0] == Dependent_activation_test_component::uuid);
        REQUIRE(deactivated_uuids[1] == Activation_test_component::uuid);
    }
}