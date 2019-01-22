#ifndef ENGINE_CORE_SYSTEM_ENTITY_HPP
#define ENGINE_CORE_SYSTEM_ENTITY_HPP

#include "engine/core/component_entity.hpp"

namespace engine::core {

//! System entity specific optimizations can be done in this class. At the
//! moment, all functionality matches the Component_entity.
class System_entity final : public Component_entity {
public:
    //! System entities start inactive.
    System_entity(Entity_id id) : Component_entity{id, false} {}
};

} // namespace engine::core

#endif /* ENGINE_CORE_SYSTEM_ENTITY_HPP */
