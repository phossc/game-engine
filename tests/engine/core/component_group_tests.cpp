#include "engine/core/component_group.hpp"

#include "engine/core/component_store.hpp"
#include "engine/core/component_types.hpp"
#include "engine/core/uuid.hpp"

#include "catch/catch.hpp"

#include <type_traits>

using engine::Uuid;
using engine::ecs::Behavior_component;
using engine::ecs::Component_group;
using engine::ecs::Component_store;
using engine::ecs::Data_component;

namespace {

struct Transform : Data_component<Transform> {
    [[maybe_unused]] static constexpr Uuid uuid{
            "ad55f08b-3d6a-467d-b443-d0095f4128c1"};
};

struct Physics : Data_component<Physics, Transform> {
    [[maybe_unused]] static constexpr Uuid uuid{
            "7598487c-b128-409c-94e9-d423bf146e65"};
};

struct Player : Behavior_component<Player, Transform, Physics> {
    [[maybe_unused]] static constexpr Uuid uuid{
            "0c14ca29-f5f7-4315-9a1a-77fd93bb5892"};
};

} // namespace

TEST_CASE("Component group types", "[component]") {
    REQUIRE(std::is_same_v<Transform::Group_tuple,
                           std::tuple<Component_store<Transform>::Index>>);

    REQUIRE(std::is_same_v<Physics::Group_tuple,
                           std::tuple<Component_store<Physics>::Index,
                                      Component_store<Transform>::Index>>);

    REQUIRE(std::is_same_v<Player::Group_tuple,
                           std::tuple<Component_store<Player>::Index,
                                      Component_store<Transform>::Index,
                                      Component_store<Physics>::Index>>);

    REQUIRE_FALSE(
            std::is_same_v<Player::Group_tuple,
                           std::tuple<Component_store<Player>::Index,
                                      Component_store<Physics>::Index,
                                      Component_store<Transform>::Index>>);
}
