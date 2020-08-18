#ifndef ENGINE_ECS_COMPONENT_TYPES_HPP
#define ENGINE_ECS_COMPONENT_TYPES_HPP

#include "engine/array_view.hpp"
#include "engine/core/uuid.hpp"
#include "engine/ecs/component_store.hpp"

#include <array>
#include <tuple>

namespace engine::ecs {

template <typename ComponentType>
struct Component_traits {
    constexpr static Uuid uuid() noexcept { return ComponentType::uuid_s(); }
    constexpr static Array_view<Uuid> deps() noexcept {
        return ComponentType::dependencies_s();
    }
};

struct Base_component {};

template <typename ComponentType, typename... Dependencies>
struct Component : Base_component {
    using Dependencies_array = std::array<Uuid, sizeof...(Dependencies)>;
    using Group_tuple = std::tuple<Store_index<ComponentType>,
                                   Store_index<Dependencies>...>;

    constexpr static Uuid uuid_s() noexcept { return ComponentType::uuid; }

    constexpr static Array_view<Uuid> dependencies_s() noexcept {
        if constexpr (dependencies.empty()) {
            return {};
        }
        else {
            return {dependencies.data(), dependencies.size()};
        }
    }

    static constexpr Dependencies_array dependencies = [] {
        Dependencies_array deps{};
        std::size_t i = 0;
        (..., (deps[i++] = Dependencies::uuid_s()));
        return deps;
    }();
};

template <typename ComponentType, typename... Dependencies>
struct Data_component : Component<ComponentType, Dependencies...> {};

struct Behavior_component_interface {
    virtual void activate() = 0;
    virtual void deactivate() = 0;
    virtual bool active() const noexcept = 0;
    virtual Uuid get_uuid() const noexcept = 0;
    virtual Array_view<Uuid> get_deps() const noexcept = 0;
};

template <typename ComponentType, typename... Dependencies>
struct Behavior_component : Component<ComponentType, Dependencies...>,
                            Behavior_component_interface {
    virtual void activate() override {}
    virtual void deactivate() override {}
    virtual bool active() const noexcept override { return false; }

    virtual Uuid get_uuid() const noexcept override final {
        return Component_traits<ComponentType>::uuid();
    }

    virtual Array_view<Uuid> get_deps() const noexcept override final {
        return Component_traits<ComponentType>::deps();
    }
};

} // namespace engine::ecs

#endif /* ENGINE_ECS_COMPONENT_TYPES_HPP */