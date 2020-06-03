#include "engine/core/component_types.hpp"
#include "engine/core/uuid.hpp"

#include "catch/catch.hpp"

using engine::Uuid;
using engine::ecs::Behavior_component;
using engine::ecs::Component_traits;
using engine::ecs::Data_component;

namespace {

struct Transform : Data_component<Transform> {
    static constexpr Uuid uuid{"e71e50ee-cc02-47be-a4fa-de406c1e7b0a"};
};

struct Render : Data_component<Render, Transform> {
    static constexpr Uuid uuid{"73b54c5a-7976-41e5-adbb-50ab76afb9e6"};
};

struct Player : Behavior_component<Player, Render, Transform> {
    static constexpr Uuid uuid{"83b5e28f-fd03-489c-809c-6d8925e6b20e"};
};

} // namespace

TEST_CASE("Component template instantiations", "[component]") {
    Uuid transform_uuid{"e71e50ee-cc02-47be-a4fa-de406c1e7b0a"};
    Uuid render_uuid{"73b54c5a-7976-41e5-adbb-50ab76afb9e6"};
    Uuid player_uuid{"83b5e28f-fd03-489c-809c-6d8925e6b20e"};

    REQUIRE(Component_traits<Transform>::uuid() == transform_uuid);
    REQUIRE(Component_traits<Render>::uuid() == render_uuid);
    REQUIRE(Component_traits<Player>::uuid() == player_uuid);

    // Direct dependencies.
    REQUIRE(Component_traits<Transform>::deps().empty());

    REQUIRE(Component_traits<Render>::deps().size() == 1);
    REQUIRE(Component_traits<Render>::deps().data()[0] ==
            Component_traits<Transform>::uuid());

    REQUIRE(Component_traits<Player>::deps().size() == 2);
    REQUIRE(Component_traits<Player>::deps().data()[0] ==
            Component_traits<Render>::uuid());
    REQUIRE(Component_traits<Player>::deps().data()[1] ==
            Component_traits<Transform>::uuid());
}