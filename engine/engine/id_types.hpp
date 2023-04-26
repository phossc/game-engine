#ifndef ENGINE_ID_TYPES_HPP
#define ENGINE_ID_TYPES_HPP

#include "jss/strong_typedef.hpp"

#include <cstdint>

namespace engine {

struct Id_property {
    template <typename Derived, typename ValueType>
    struct mixin {
        static constexpr Derived invalid() { return Derived{-1}; }
        static constexpr Derived any() { return Derived{-5}; }
    };
};

using Entity_id = jss::strong_typedef<
        struct Entity_id_tag, std::int64_t, jss::strong_typedef_properties::comparable,
        jss::strong_typedef_properties::hashable, jss::strong_typedef_properties::incrementable,
        jss::strong_typedef_properties::streamable, Id_property>;

} // namespace engine

#endif /* ENGINE_ID_TYPES_HPP */