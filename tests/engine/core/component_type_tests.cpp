#include "engine/core/component_types.hpp"
#include "engine/core/uuid.hpp"

#include "catch/catch.hpp"

TEST_CASE("Component template instantiations", "[component]") {
    using engine::Uuid;
    using engine::ecs::Behavior_component;
    using engine::ecs::Data_component;

    constexpr Uuid transform_uuid{"e71e50ee-cc02-47be-a4fa-de406c1e7b0a"};
    struct Transform
        : Data_component<transform_uuid.upper(), transform_uuid.lower()> {};

    constexpr Uuid render_uuid{"73b54c5a-7976-41e5-adbb-50ab76afb9e6"};
    struct Render
        : Data_component<render_uuid.upper(), render_uuid.lower(), Transform> {
    };

    constexpr Uuid player_uuid{"83b5e28f-fd03-489c-809c-6d8925e6b20e"};
    struct Player : Behavior_component<player_uuid.upper(), player_uuid.lower(),
                                       Render, Transform> {};

    REQUIRE(Transform::uuid_s() == transform_uuid);
    REQUIRE(Render::uuid_s() == render_uuid);
    REQUIRE(Player::uuid_s() == player_uuid);

    // Direct dependencies.
    REQUIRE(Transform::dependencies_s().empty());
    REQUIRE(Render::dependencies_s().size() == 1);
    REQUIRE(Render::dependencies_s().data()[0] == Transform::uuid_s());
    REQUIRE(Player::dependencies_s().size() == 2);
    REQUIRE(Player::dependencies_s().data()[0] == Render::uuid_s());
    REQUIRE(Player::dependencies_s().data()[1] == Transform::uuid_s());
}