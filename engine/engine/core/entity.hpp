#ifndef ENGINE_CORE_ENTITY_HPP
#define ENGINE_CORE_ENTITY_HPP

#include "engine/core/uuid.hpp"
#include "engine/id_types.hpp"

namespace engine {

class Component;

//! Specialized entities derive from this class to integrate with the ECS.
//! Entities should only be created and destroyed through the ECS.
class Entity {
public:
    Entity(Entity_id id) : id_(id) {}
    virtual ~Entity() = default;

    //! Update is called by the ECS if the entity has an update scheduled.
    virtual void update() {}

    //! Activate should not activate the entity immediatly but rather do it in
    //! the next ECS update. Components are activated in the order of their
    //! dependencies.
    virtual void activate() {}

    //! Deactivate should not deactivate the entity immediatly but rather do it
    //! in the next ECS update. Components are deactivated in the reverse order
    //! of their dependencies.
    virtual void deactivate() {}

    //! Creates a component with the specified UUID and all of its dependencies
    //! that are not already present in the entity.
    //!
    //! Newly created components are activated in the order of their
    //! dependencies in the next ECS update if the entity is active.
    //!
    //! \return a pointer to the component with the specified UUID and if
    //! creation fails then nullptr.
    virtual Component* create(Uuid) { return nullptr; }

    //! Calls create(ComponentType::uuid_s()).
    template <typename ComponentType>
    ComponentType* create() {
        return static_cast<ComponentType*>(create(ComponentType::uuid_s()));
    }

    //! Gets the component with the specified UUID.
    //!
    //! \return a pointer to the component or nullptr if it does not exist.
    virtual Component* get(Uuid) { return nullptr; };

    //! Calls get(ComponentType::uuid_s()).
    template <typename ComponentType>
    ComponentType* get() {
        return static_cast<ComponentType*>(get(ComponentType::uuid_s()));
    }

    //! Whether the entity is active or not.
    virtual bool active() const noexcept { return false; }

    Entity_id id() const noexcept { return id_; }

private:
    Entity_id id_;
};

} // namespace engine

#endif /* ENGINE_CORE_ENTITY_HPP */
