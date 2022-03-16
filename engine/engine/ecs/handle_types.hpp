#ifndef ENGINE_ECS_HANDLE_TYPES_HPP
#define ENGINE_ECS_HANDLE_TYPES_HPP

#include "jss/strong_typedef.hpp"

#include <cstddef>
#include <cstdint>

namespace engine::ecs {

struct Id_property {
    template <typename Derived, typename ValueType>
    struct mixin {
        static constexpr Derived invalid() { return Derived{-1}; }
        static constexpr Derived any() { return Derived{-5}; }
    };
};

using Entity_id =
        jss::strong_typedef<struct Entity_id_tag, std::int64_t,
                            jss::strong_typedef_properties::comparable,
                            jss::strong_typedef_properties::hashable,
                            jss::strong_typedef_properties::incrementable,
                            jss::strong_typedef_properties::streamable,
                            Id_property>;

/// The order in which a component appears in the topologically sorted
/// dependency graph of all components.
using Dependency_order =
        jss::strong_typedef<struct Dependency_order_tag, std::int32_t,
                            jss::strong_typedef_properties::comparable,
                            jss::strong_typedef_properties::hashable,
                            jss::strong_typedef_properties::incrementable,
                            jss::strong_typedef_properties::streamable>;

/// Typed component index that can only be used with component stores of
/// ComponentType components.
template <typename ComponentType>
using Typed_component_index =
        jss::strong_typedef<ComponentType, std::size_t,
                            jss::strong_typedef_properties::comparable,
                            jss::strong_typedef_properties::hashable,
                            jss::strong_typedef_properties::streamable>;

/// Generic component index that can be used with all component stores.
using Component_index = Typed_component_index<struct Component_index_tag>;

} // namespace engine::ecs

#endif /* ENGINE_ECS_HANDLE_TYPES_HPP */