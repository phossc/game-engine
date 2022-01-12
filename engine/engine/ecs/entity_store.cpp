#include "engine/ecs/entity_store.hpp"

#include <algorithm>
#include <cassert>

namespace engine::ecs {

Entity_store::Entity_range Entity_store::create(Array_view<Entry> components) {
    Entity_range result_range{0, 0};

    auto iter = available_indices_.find(components.size());
    if (iter != std::end(available_indices_)) {
        auto insert_index = iter->second;
        result_range.first = insert_index;
        result_range.second = insert_index + components.size();
        std::copy(std::begin(components), std::end(components),
                  std::begin(component_indices_) + insert_index);
        available_indices_.erase(iter);
    }
    else {
        result_range.first = component_indices_.size();
        result_range.second = component_indices_.size() + components.size();
        component_indices_.reserve(component_indices_.size() +
                                   components.size());
        std::copy(std::begin(components), std::end(components),
                  std::back_inserter(component_indices_));
    }

    return result_range;
}

void Entity_store::erase(Entity_range range) {
    assert(range.first >= 0 && range.first < component_indices_.size());
    assert(range.second >= 0 && range.second <= component_indices_.size());

    available_indices_.emplace(range.second - range.first, range.first);
}

Array_view<Entity_store::Entry>
Entity_store::get_entity_components(Entity_range range) const noexcept {
    assert(range.first >= 0 && range.first < component_indices_.size());
    assert(range.second >= 0 && range.second <= component_indices_.size());

    return Array_view<Entry>{component_indices_.data() + range.first,
                             range.second - range.first};
}

} // namespace engine::ecs