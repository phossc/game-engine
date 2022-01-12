#ifndef ENGINE_ECS_ENTITY_STORE_HPP
#define ENGINE_ECS_ENTITY_STORE_HPP

#include "engine/array_view.hpp"
#include "engine/ecs/handle_types.hpp"

#include <unordered_map>
#include <utility>
#include <vector>

namespace engine::ecs {

class Entity_store {
public:
    using Index = std::size_t;
    using Entity_range = std::pair<Index, Index>;
    using Entry = std::pair<Dependency_order, Component_index>;

    /// Creates an entity with the provided components. Components are not
    /// activated. Entities may not contain duplicate components.
    Entity_range create(Array_view<Entry> components);

    /// Deletes an entity from the store. Components are not deactivated and not
    /// deleted. The range must denote an entity.
    void erase(Entity_range range);

    /// Provides a view into the entity described by range, exposing its
    /// components. The range must denote an entity. Entity creation can
    /// invalidate the returned array view.
    Array_view<Entry> get_entity_components(Entity_range range) const noexcept;

private:
    std::vector<Entry> component_indices_;

    using Entity_size = int;
    std::unordered_multimap<Entity_size, Index> available_indices_;
};

} // namespace engine::ecs

#endif /* ENGINE_ECS_ENTITY_STORE_HPP */