#ifndef ENGINE_CORE_COMPONENT_TYPES_HPP
#define ENGINE_CORE_COMPONENT_TYPES_HPP

#include "engine/array_view.hpp"
#include "engine/core/uuid.hpp"

#include <array>

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

template <typename ComponentType, typename... Dependencies>
struct Behavior_component : Component<ComponentType, Dependencies...> {};

} // namespace engine::ecs

#endif /* ENGINE_CORE_COMPONENT_TYPES_HPP */