#ifndef ENGINE_ECS_ECS_HPP
#define ENGINE_ECS_ECS_HPP

#include "engine/core/uuid.hpp"
#include "engine/ecs/component_grouping.hpp"
#include "engine/ecs/component_store.hpp"
#include "engine/ecs/component_types.hpp"
#include "engine/ecs/entity_builder.hpp"
#include "engine/ecs/entity_store.hpp"
#include "engine/ecs/handle_types.hpp"

#include <functional>
#include <memory>
#include <unordered_map>

namespace engine::ecs {

class Ecs final {
public:
    /// The component registrar forwards component registration calls to the
    /// ECS. It is passed to client code by the ECS and prevents registration of
    /// components after entity creation.
    class Component_registrar final {
    public:
        explicit Component_registrar(Ecs& ecs) : ecs_(ecs) {
            // The reference to the ECS should not be used in this constructor
            // as the ECS might not be fully constructed yet.
        }

        template <typename ComponentType>
        void register_component() const {
            ecs_.register_component<ComponentType>();
        }

    private:
        Ecs& ecs_;
    };

    Ecs() : component_registrar_(*this) {}

    [[nodiscard]] Entity_builder<Ecs> entity() {
        return Entity_builder<Ecs>(*this);
    }

    template <typename ComponentType>
    [[nodiscard]] Component_store<ComponentType>& component_store() const {
        return static_cast<Component_store<ComponentType>&>(
                *component_stores_.at(Component_traits<ComponentType>::uuid()));
    }

    template <typename ComponentType>
    [[nodiscard]] Component_grouping<ComponentType, Ecs>&
    component_grouping() const {
        return static_cast<Component_grouping<ComponentType, Ecs>&>(
                *component_groupings_.at(
                        Component_traits<ComponentType>::uuid()));
    }

    template <typename ComponentType>
    [[nodiscard]] Dependency_order dependency_order() const {
        return dependency_ordering_.at(Component_traits<ComponentType>::uuid());
    }

    /// Components are registered through the Component_registrar passed to
    /// the registration function. After registration is done, it is not
    /// possible to register more components. This limitation is likely to
    /// change in the future.
    void
    register_components(const std::function<void(const Component_registrar&)>&
                                registration_function);

    // Finishes the creation of entities that have been set up before this call.
    // Should be called at the beginning of every frame as it can invalidate
    // references into component groupings and the entity store.
    void build_new_entities();

private:
    friend struct Entity_builder<Ecs>;

    /// Used by the Entity_builder class to schedule entity creation.
    Entity_id schedule_entity_builder(Entity_builder<Ecs> builder);

    /// Used by the Entity_builder class to get a uuid from a dependency order.
    [[nodiscard]] Uuid uuid_from(Dependency_order dependency_order) {
        return dependency_order_to_uuid_.at(dependency_order);
    }

    template <typename ComponentType>
    void register_component() {
        auto uuid = Component_traits<ComponentType>::uuid();
        component_stores_.emplace(
                uuid, std::make_unique<Component_store<ComponentType>>());
        component_groupings_.emplace(
                uuid, std::make_unique<Component_grouping<ComponentType, Ecs>>(
                              *this));
        dependency_graph_[uuid] = Component_traits<ComponentType>::deps();
    }

    /// A number indicating the order of dependencies will be assigned to every
    /// component. Components can then be ordered by their dependencies by
    /// simply sorting them based on their dependency order.
    /// Returns false on circular dependencies and non existent dependencies.
    bool calculate_dependency_ordering();

    std::unordered_map<Uuid, std::unique_ptr<Component_store_base>>
            component_stores_;
    std::unordered_map<Uuid, std::unique_ptr<Component_grouping_base>>
            component_groupings_;

    std::unordered_map<Uuid, Array_view<Uuid>> dependency_graph_;
    std::unordered_map<Uuid, Dependency_order> dependency_ordering_;
    std::unordered_map<Dependency_order, Uuid> dependency_order_to_uuid_;

    Entity_store entity_store_;
    std::unordered_map<Entity_id, Entity_store::Entity_range> entities_;
    std::vector<std::pair<Entity_id, Entity_builder<Ecs>>>
            scheduled_entity_builders_;

    /// The entity id used for the next entity being created.
    Entity_id current_entity_id_{0};

    bool registration_done_ = false;
    Component_registrar component_registrar_;
};

} // namespace engine::ecs

#endif /* ENGINE_ECS_ECS_HPP */