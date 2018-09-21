#ifndef ENGINE_CORE_ENTITY_HPP
#define ENGINE_CORE_ENTITY_HPP

#include <engine/core/entity_id.hpp>

namespace engine::core {

//! Specialized entities derive from this class to integrate with the ECS.
class Entity {
public:
    Entity(Entity_id id) : id_(id) {}
    virtual ~Entity() = default;

    //! Update is called by the ECS if the entity has an update scheduled.
    virtual void update() {}

    //! Activate should not activate the entity immediatly but rather do it in
    //! the next ECS update.
    virtual void activate() {}

    //! Deactivate should not deactivate the entity immediatly but rather do it
    //! in the next ECS update.
    virtual void deactivate() {}

    virtual Entity_id id() const noexcept { return id_; }

private:
    Entity_id id_;
};

} // namespace engine::core

#endif /* ENGINE_CORE_ENTITY_HPP */
