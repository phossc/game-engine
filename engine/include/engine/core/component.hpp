#ifndef ENGINE_CORE_COMPONENT_HPP
#define ENGINE_CORE_COMPONENT_HPP

namespace engine::core {

class Entity;

class Component {
public:
    virtual ~Component() = default;

    virtual void activate() = 0;
    virtual void deactivate() = 0;
    virtual bool active() = 0;

    void set_entity(Entity* entity) noexcept { entity_ = entity; }

    Entity* entity() const noexcept { return entity_; }

private:
    Entity* entity_ = nullptr;
};

} // namespace engine::core

#endif /* ENGINE_CORE_COMPONENT_HPP */
