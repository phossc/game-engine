#ifndef ENGINE_CORE_COMPONENT_ENTITY_HPP
#define ENGINE_CORE_COMPONENT_ENTITY_HPP

#include <memory>
#include <unordered_map>
#include <vector>

#include <engine/core/component_uuid.hpp>
#include <engine/core/entity.hpp>

namespace engine::core {

class Component;

class Component_entity : public Entity {
public:
    Component_entity(Entity_id id) : Entity{id} {}
    virtual ~Component_entity();

    //! Creates a component and all of its dependencies that are not already
    //! present in the entity.
    //!
    //! The newly created components are added to the entity immediatly.
    //! Activation of the added components happens in the next ECS update if the
    //! entity is active, otherwise they remain inactive until activate() is
    //! called on the entity. Activation order is based on the dependencies of
    //! the components. Does not do anything if the component already exists or
    //! creation fails.
    //!
    //! \return a pointer to the component with the specified UUID and if
    //! creation fails then nullptr.
    Component* create_component(Component_uuid uuid);

    //! Calls create_component(ComponentType::s_uuid()).
    template <typename ComponentType>
    ComponentType* create_component() {
        return static_cast<ComponentType*>(
                create_component(ComponentType::s_uuid()));
    }

    //! Gets the component with the specified UUID.
    //!
    //! \return a pointer to the component or nullptr if it does not exist.
    Component* get(Component_uuid uuid);

    //! Calls get(ComponentType::s_uuid()).
    template <typename ComponentType>
    ComponentType* get() {
        return static_cast<ComponentType*>(get(ComponentType::s_uuid()));
    }

    //! Activates or deactivates all components in the entity depending on which
    //! was scheduled last. If neither activation nor deactivation is scheduled
    //! and new components were added, then activate them if the entity is
    //! active otherwise don't.
    void update() override final;

    //! Schedule entity activation for the next ECS update.
    //!
    //! Overrides any previously scheduled activations or deactivations.
    void activate() override final;

    //! Schedule entity deactivation for the next ECS update.
    //!
    //! Overrides any previously scheduled activations or deactivations.
    void deactivate() override final;

private:
    void perform_activation();
    void perform_deactivation();

    enum class Task { none, deactivate, activate, activate_new_components };
    Task scheduled_task_ = Task::none;
    bool active_ = false;

    std::unordered_map<Component_uuid, std::unique_ptr<Component>> components_;
    std::vector<Component_uuid> component_creation_order_;
    std::size_t new_components_start_index_ = 0;
};

} // namespace engine::core

#endif /* ENGINE_CORE_COMPONENT_ENTITY_HPP */