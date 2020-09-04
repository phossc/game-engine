#include "engine/ecs/component_group.hpp"

#include "engine/core/uuid.hpp"
#include "engine/ecs/component_store.hpp"
#include "engine/ecs/component_types.hpp"

#include "catch/catch.hpp"

#include <algorithm>
#include <string>
#include <type_traits>
#include <vector>

using engine::Uuid;
using engine::ecs::Behavior_component;
using engine::ecs::Component_group;
using engine::ecs::Component_store;
using engine::ecs::Data_component;

namespace {

struct Transform : Data_component<Transform> {
    [[maybe_unused]] static constexpr Uuid uuid{
            "ad55f08b-3d6a-467d-b443-d0095f4128c1"};

    Transform(std::string id_) : id(std::move(id_)) {}
    std::string id;
};

struct Physics : Data_component<Physics, Transform> {
    [[maybe_unused]] static constexpr Uuid uuid{
            "7598487c-b128-409c-94e9-d423bf146e65"};

    Physics(std::string id_) : id(std::move(id_)) {}
    std::string id;
};

struct Player : Behavior_component<Player, Transform, Physics> {
    [[maybe_unused]] static constexpr Uuid uuid{
            "0c14ca29-f5f7-4315-9a1a-77fd93bb5892"};

    Player(std::string id_) : id(std::move(id_)) {}
    std::string id;
};

/// Used in the else clause of an if constexpr to fail compilation with
/// static_assert.
template <typename T>
struct dependent_false : std::false_type {};

struct Store_provider {
    template <typename ComponentType>
    Component_store<ComponentType>& component_store() {
        if constexpr (std::is_same_v<ComponentType, Transform>) {
            return transform_components;
        }
        else if constexpr (std::is_same_v<ComponentType, Physics>) {
            return physics_components;
        }
        else if constexpr (std::is_same_v<ComponentType, Player>) {
            return player_components;
        }
        else {
            static_assert(dependent_false<ComponentType>::value,
                          "The store provider does not provide a "
                          "component store for that type.");
        }
    }

    Component_store<Transform> transform_components;
    Component_store<Physics> physics_components;
    Component_store<Player> player_components;
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

TEST_CASE("Component group iteration", "[component]") {
    Store_provider store_provider;
    Component_group<Transform, Store_provider> transform_groups(store_provider);
    Component_group<Physics, Store_provider> physics_groups(store_provider);
    Component_group<Player, Store_provider> player_groups(store_provider);

    // i_tr1: index of transform 1
    // i_ph1: index of physics 1
    // i_pl1: index of player 1

    // Entities:
    // 1. [transform1]
    // 2. [transform2, physics1, player1]
    // 3. [transform3, physics2]

    auto i_tr1 = store_provider.transform_components.insert({"transform1"});
    transform_groups.add_group(std::make_tuple(i_tr1));

    auto i_tr2 = store_provider.transform_components.insert({"transform2"});
    transform_groups.add_group(std::make_tuple(i_tr2));

    auto i_tr3 = store_provider.transform_components.insert({"transform3"});
    transform_groups.add_group(std::make_tuple(i_tr3));

    auto i_ph1 = store_provider.physics_components.insert({"physics1"});
    physics_groups.add_group(std::make_tuple(i_ph1, i_tr2));

    auto i_ph2 = store_provider.physics_components.insert({"physics2"});
    physics_groups.add_group(std::make_tuple(i_ph2, i_tr3));

    auto i_pl1 = store_provider.player_components.insert({"player1"});
    player_groups.add_group(std::make_tuple(i_pl1, i_tr2, i_ph1));

    std::vector<std::string> transform_group_transforms{
            "transform1", "transform2", "transform3"};

    std::vector<std::string> physics_group_physics{"physics1", "physics2"};
    std::vector<std::string> physics_group_transforms{"transform2",
                                                      "transform3"};

    std::vector<std::string> player_group_players{"player1"};
    std::vector<std::string> player_group_transforms{"transform2"};
    std::vector<std::string> player_group_physics{"physics1"};

    auto tr_view_tr = transform_groups.view<Transform>();
    REQUIRE(std::equal(tr_view_tr.begin(), tr_view_tr.end(),
                       transform_group_transforms.begin(),
                       transform_group_transforms.end(),
                       [](auto component_tuple, const auto& component_id) {
                           auto [component_ptr] = component_tuple;
                           return component_ptr->id == component_id;
                       }));

    auto ph_view_ph = physics_groups.view<Physics>();
    REQUIRE(std::equal(ph_view_ph.begin(), ph_view_ph.end(),
                       physics_group_physics.begin(),
                       physics_group_physics.end(),
                       [](auto component_tuple, const auto& component_id) {
                           auto [component_ptr] = component_tuple;
                           return component_ptr->id == component_id;
                       }));

    auto ph_view_tr = physics_groups.view<Transform>();
    REQUIRE(std::equal(ph_view_tr.begin(), ph_view_tr.end(),
                       physics_group_transforms.begin(),
                       physics_group_transforms.end(),
                       [](auto component_tuple, const auto& component_id) {
                           auto [component_ptr] = component_tuple;
                           return component_ptr->id == component_id;
                       }));

    auto pl_view_pl = player_groups.view<Player>();
    REQUIRE(std::equal(pl_view_pl.begin(), pl_view_pl.end(),
                       player_group_players.begin(), player_group_players.end(),
                       [](auto component_tuple, const auto& component_id) {
                           auto [component_ptr] = component_tuple;
                           return component_ptr->id == component_id;
                       }));

    auto pl_view_tr = player_groups.view<Transform>();
    REQUIRE(std::equal(pl_view_tr.begin(), pl_view_tr.end(),
                       player_group_transforms.begin(),
                       player_group_transforms.end(),
                       [](auto component_tuple, const auto& component_id) {
                           auto [component_ptr] = component_tuple;
                           return component_ptr->id == component_id;
                       }));

    auto pl_view_ph = player_groups.view<Physics>();
    REQUIRE(std::equal(pl_view_ph.begin(), pl_view_ph.end(),
                       player_group_physics.begin(), player_group_physics.end(),
                       [](auto component_tuple, const auto& component_id) {
                           auto [component_ptr] = component_tuple;
                           return component_ptr->id == component_id;
                       }));
}
