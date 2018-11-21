#ifndef ENGINE_CORE_COMPONENT_HPP
#define ENGINE_CORE_COMPONENT_HPP

namespace engine::core {

class Entity;

class Component {
public:
    virtual ~Component() = default;

    virtual void activate() = 0;
    virtual void deactivate() = 0;
    bool active() const noexcept { return active_; }

    void set_entity(Entity* entity) noexcept { entity_ = entity; }

    Entity* entity() const noexcept { return entity_; }

    friend class Component_entity;

private:
    Entity* entity_ = nullptr;
    bool active_ = false;
};

} // namespace engine::core

#endif /* ENGINE_CORE_COMPONENT_HPP */
