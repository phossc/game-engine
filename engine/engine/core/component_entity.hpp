#ifndef ENGINE_CORE_COMPONENT_ENTITY_HPP
#define ENGINE_CORE_COMPONENT_ENTITY_HPP

#include "engine/core/component.hpp"
#include "engine/core/entity.hpp"

#include <memory>
#include <unordered_map>
#include <vector>

namespace engine::core {

class Component_entity : public Entity {
public:
    Component_entity(Entity_id id) : Entity{id} {}
    Component_entity(Entity_id id, bool active) : Entity{id}, active_{active} {}

    virtual ~Component_entity();

    Component* create(Uuid uuid) override final;
    Component* get(Uuid uuid) override final;

    //! Prevent overloads from being hidden.
    using Entity::create;
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

    std::unordered_map<Uuid, std::unique_ptr<Component>> components_;
    std::vector<Uuid> component_creation_order_;
    std::size_t new_components_start_index_ = 0;
};

} // namespace engine::core

#endif /* ENGINE_CORE_COMPONENT_ENTITY_HPP */