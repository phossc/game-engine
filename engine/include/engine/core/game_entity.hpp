#ifndef ENGINE_CORE_GAME_ENTITY_HPP
#define ENGINE_CORE_GAME_ENTITY_HPP

#include <deque>

#include <engine/core/component_entity.hpp>

namespace engine::core {

//! This entity makes it possible to arrange hierarchies of entities. If an
//! entity in the hierarchy is destructed, then it detaches all its children and
//! detaches itself from the parent. Following a parent entity access, a
//! component access on that parent is usually desired. This only makes sense
//! when the parent entity is active and the parent() accessor on a game entity
//! therefore returns nullptr if its parent is not active.
class Game_entity final : public Component_entity {
public:
    //! Game entities start active.
    Game_entity(Entity_id id) : Component_entity{id, true} {}

    ~Game_entity();

    //! Makes this entity the parent of child.
    void attach(Game_entity* child) noexcept;

    //! Removes child from this parent entity.
    void detach(Game_entity* child) noexcept;

    //! \return a pointer to the parent entity if it exists and is active.
    //! Otherwise nullptr.
    Game_entity* parent() const noexcept;

private:
    Game_entity* parent_ = nullptr;
    std::deque<Game_entity*> children_;
};

} // namespace engine::core

#endif /* ENGINE_CORE_GAME_ENTITY_HPP */