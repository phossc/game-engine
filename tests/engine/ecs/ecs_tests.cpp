#include "engine/ecs/ecs.hpp"

#include "catch/catch.hpp"

using engine::Uuid;
using engine::ecs::Behavior_component;
using engine::ecs::Data_component;
using engine::ecs::Ecs;

namespace {

struct Transform_component : Data_component<Transform_component> {
    static constexpr Uuid uuid{"8a8fe1d9-7204-495b-ba00-d499825af964"};
};

struct Mesh_component : Data_component<Mesh_component, Transform_component> {
    static constexpr Uuid uuid{"53b49809-eb86-43c5-895d-d24f74849cf3"};
};

struct Player_component : Behavior_component<Player_component, Mesh_component,
                                             Transform_component> {
    static constexpr Uuid uuid{"15ccd6bf-cbb8-4ace-8fbb-7ee93fd3e050"};
};

struct Circular_dependency_one
    : Data_component<Circular_dependency_one, struct Circular_dependency_two> {
    static constexpr Uuid uuid{"c0379bfa-241e-463d-9632-c00a5612660c"};
};

struct Circular_dependency_two
    : Data_component<Circular_dependency_two, Player_component,
                     Circular_dependency_one> {
    static constexpr Uuid uuid{"a23bd605-445c-44b4-8bd2-31ee41403b8d"};
};

struct Unregistered_component : Data_component<Unregistered_component> {
    static constexpr Uuid uuid{"63ec9adf-4c0d-460d-817a-421d7d2bf0fb"};
};

} // namespace

TEST_CASE("ECS component registration", "[ecs]") {
    SECTION("Valid dependency graph") {
        Ecs ecs;
        REQUIRE_NOTHROW(ecs.register_components(
                [](const Ecs::Component_registrar& registrar) {
                    registrar.register_component<Transform_component>();
                    registrar.register_component<Mesh_component>();
                    registrar.register_component<Player_component>();
                }));
    }

    SECTION("Circular dependencies") {
        Ecs ecs;
        REQUIRE_THROWS(ecs.register_components(
                [](const Ecs::Component_registrar& registrar) {
                    registrar.register_component<Transform_component>();
                    registrar.register_component<Mesh_component>();
                    registrar.register_component<Player_component>();
                    registrar.register_component<Circular_dependency_one>();
                    registrar.register_component<Circular_dependency_two>();
                }));
    }

    SECTION("Missing dependency") {
        Ecs ecs;
        REQUIRE_THROWS(ecs.register_components(
                [](const Ecs::Component_registrar& registrar) {
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
        ecs.entity()
                .with<Transform_component>()
                .with<Transform_component>()
                .build();
        REQUIRE_THROWS(ecs.lifecycle_update());
    }
}
