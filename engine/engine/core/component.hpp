#ifndef ENGINE_CORE_COMPONENT_HPP
#define ENGINE_CORE_COMPONENT_HPP

#include "engine/array_view.hpp"
#include "engine/core/entity.hpp"
#include "engine/core/uuid.hpp"

#include <array>
#include <cassert>
#include <tuple>

namespace engine::core {

class Component {
public:
    virtual ~Component() = default;

    virtual Uuid uuid() const noexcept = 0;
    virtual Array_view<Uuid> dependencies() const noexcept = 0;

    virtual void activate() {}
    virtual void deactivate() {}

    //! Returns a component dependency from the containing entity.
    Component* dependency(Uuid uuid) const {
        assert(entity_ != nullptr);
        return entity_->get(uuid);
    }

    //! Returns a component dependency from the containing entity.
    template <typename ComponentType>
    ComponentType* dependency() const {
        assert(entity_ != nullptr);
        return entity_->get<ComponentType>();
    }

    void set_entity(Entity* entity) noexcept { entity_ = entity; }
    Entity* entity() const noexcept { return entity_; }

    bool active() const noexcept { return active_; }

    friend class Component_entity;

private:
    Entity* entity_ = nullptr;
    bool active_ = false;
};

} // namespace engine::core

#define COMPONENT(uuid_string) UUID(uuid_string)

#define DEPENDENCIES_IMPL(...)                                                 \
    static std::array<                                                         \
            engine::core::Uuid,                                                \
            std::tuple_size_v<decltype(std::make_tuple(__VA_ARGS__))>>         \
            uuids{__VA_ARGS__};                                                \
                                                                               \
    if (!uuids.empty()) {                                                      \
        return {uuids.data(), uuids.size()};                                   \
    }                                                                          \
                                                                               \
    return {};

#define DEPENDENCIES(...)                                                      \
    static engine::Array_view<engine::core::Uuid> s_dependencies() noexcept {  \
        DEPENDENCIES_IMPL(__VA_ARGS__);                                        \
    }                                                                          \
                                                                               \
    virtual engine::Array_view<engine::core::Uuid> dependencies()              \
            const noexcept override {                                          \
        return s_dependencies();                                               \
    }

#endif /* ENGINE_CORE_COMPONENT_HPP */