#include "engine/core/component_store.hpp"

#include "engine/core/component_types.hpp"
#include "engine/core/uuid.hpp"

#include "catch/catch.hpp"

using engine::Uuid;
using engine::ecs::Component_store;
using engine::ecs::Data_component;

namespace {

struct Transform : Data_component<Transform> {
    [[maybe_unused]] static constexpr Uuid uuid{
            "a5316888-a19a-4dce-ac7b-2266370ecb83"};

    Transform(int id_) : id(id_) {}
    int id;
};

} // namespace

TEST_CASE("Basic component store operations", "[component]") {
    Component_store<Transform> store;
    auto index = store.insert(Transform{1});
    REQUIRE(store[index].id == 1);
    store.remove(index);

    index = store.insert(Transform{0});
    REQUIRE(store[index].id == 0);
    store.remove(index);

    auto index1 = store.insert(Transform{4});
    auto index2 = store.insert(Transform{5});
    REQUIRE(index1 != index2);
    REQUIRE(store[index1].id == 4);
    REQUIRE(store[index2].id == 5);
    store.remove(index1);
    store.remove(index2);
}
