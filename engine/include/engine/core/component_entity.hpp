#ifndef ENGINE_CORE_COMPONENT_ENTITY_HPP
#define ENGINE_CORE_COMPONENT_ENTITY_HPP

#include <memory>
#include <unordered_map>
#include <vector>

#include <engine/core/entity.hpp>

namespace engine::core {

class Component;

class Component_entity : public Entity {
public:
    Component_entity(Entity_id id) : Entity{id} {}
    virtual ~Component_entity();

    Component* create_component(Component_uuid uuid) override final;
    Component* get(Component_uuid uuid) override final;

    //! Prevent overloads from being hidden.
    using Entity::create_component;
    using Entity::get;

    //! Updates the entity based on the scheduled task.
    void update() override final;

    //! Schedules entity activation for the next ECS update.
    //!
    //! Overrides scheduled tasks and only activates inactive components.
    void activate() override final;

    //! Schedules entity deactivation for the next ECS update.
    //!
    //! Overrides scheduled tasks and only deactivates active components.
    void deactivate() override final;

    bool active() const noexcept override final { return active_; }

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