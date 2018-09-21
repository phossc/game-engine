#ifndef ENGINE_CORE_ECS_HPP
#define ENGINE_CORE_ECS_HPP

#include <deque>
#include <engine/core/entity.hpp>
#include <engine/core/entity_id.hpp>
#include <memory>
#include <unordered_map>

namespace engine::core {

class Ecs final {
public:
    //! Creates an entity of the specified type.
    //!
    //! \return a non-owning raw pointer to the created entity.
    template <typename EntityType>
    EntityType* create_entity();

    //! Retrieves the entity with the provided id.
    //!
    //! \return a non-owning raw pointer to the entity if it exists otherwise
    //! nullptr.
    Entity* get_entity(Entity_id entity_id);

    //! Schedules entity deletion in the next ECS update.
    //!
    //! \sa update()
    void schedule_deletion(Entity_id entity_id);

    //! Schedules an entity update in the next ECS update.
    //!
    //! \sa update(), Entity::update()
    void schedule_update(Entity_id entity_id);

    //! Entities scheduled for deletion are deleted and update is called on all
    //! entities who have an update scheduled.
    void update();

private:
    //! Data member holding the id that a newly created entity will get.
    //!
    //! The value_type is an std::int64_t and the id is advanced by incrementing
    //! it. Thus we will run into trouble if entities are created continually
    //! for a few billion years without an engine restart. We'll take that risk.
    Entity_id::value_type next_id_ = 0;

    //! Entities currently in the system.
    std::unordered_map<Entity_id, std::unique_ptr<Entity>> entities_;

    std::deque<Entity_id> entities_scheduled_for_update_;
    std::deque<Entity_id> entities_being_updated_;

    std::deque<Entity_id> entities_scheduled_for_deletion_;
    std::deque<Entity_id> entities_being_deleted_;
};

template <typename EntityType>
EntityType* Ecs::create_entity() {
    Entity_id id{next_id_++};
    auto p = entities_.emplace(id, std::make_unique<EntityType>(id));
    return static_cast<EntityType*>(p.first->second.get());
}

} // namespace engine::core

#endif /* ENGINE_CORE_ECS_HPP */
