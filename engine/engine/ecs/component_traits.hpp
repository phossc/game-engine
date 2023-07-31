#ifndef ENGINE_ECS_COMPONENT_TRAITS_HPP
#define ENGINE_ECS_COMPONENT_TRAITS_HPP

#include "engine/array_view.hpp"
#include "engine/core/uuid.hpp"

namespace engine::ecs {

template <typename ComponentType>
struct Component_traits {
    constexpr static Uuid uuid() noexcept { return ComponentType::uuid_s(); }

    constexpr static Array_view<Uuid> deps() noexcept { return ComponentType::dependencies_s(); }

    constexpr static bool sort_deps() noexcept { return ComponentType::sort_dependencies(); }

    constexpr static bool behavioral() noexcept { return ComponentType::behavioral(); }
};

} // namespace engine::ecs

#endif /* ENGINE_ECS_COMPONENT_TRAITS_HPP */
