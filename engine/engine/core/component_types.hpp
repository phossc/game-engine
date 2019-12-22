#ifndef ENGINE_CORE_COMPONENT_TYPES_HPP
#define ENGINE_CORE_COMPONENT_TYPES_HPP

#include "engine/array_view.hpp"
#include "engine/core/uuid.hpp"

#include <array>

namespace engine::ecs {

struct Base_component {};

template <std::uint64_t Upper, std::uint64_t Lower, typename... Dependencies>
struct Component : Base_component {
    constexpr static Uuid uuid_s() noexcept {
        constexpr Uuid uuid{Upper, Lower};
        return uuid;
    }

    constexpr static Array_view<Uuid> dependencies_s() noexcept {
        static constexpr auto uuids = [] {
            std::array<Uuid, sizeof...(Dependencies)> deps;
            std::size_t i = 0;
            (..., (deps[i++] = Dependencies::uuid_s()));
            return deps;
        }();

        if constexpr (uuids.empty()) {
            return {};
        }
        else {
            return {uuids.data(), uuids.size()};
        }
    }

    Uuid uuid() const noexcept { return uuid_s(); }
    Array_view<Uuid> dependencies() const noexcept { return dependencies_s(); }
};

template <std::uint64_t Upper, std::uint64_t Lower, typename... Dependencies>
struct Data_component : Component<Upper, Lower, Dependencies...> {};

template <std::uint64_t Upper, std::uint64_t Lower, typename... Dependencies>
struct Behavior_component : Component<Upper, Lower, Dependencies...> {};

} // namespace engine::ecs

#endif /* ENGINE_CORE_COMPONENT_TYPES_HPP */