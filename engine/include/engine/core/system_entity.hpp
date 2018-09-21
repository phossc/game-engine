#ifndef ENGINE_CORE_SYSTEM_ENTITY_HPP
#define ENGINE_CORE_SYSTEM_ENTITY_HPP

#include <engine/core/entity.hpp>

namespace engine::core {

class System_entity : public Entity {
public:
    System_entity(Entity_id id) : Entity{id} {}
};

} // namespace engine::core

#endif /* ENGINE_CORE_SYSTEM_ENTITY_HPP */
