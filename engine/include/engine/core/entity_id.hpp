#ifndef ENGINE_CORE_ENTITY_ID_HPP
#define ENGINE_CORE_ENTITY_ID_HPP

#include <cstdint>
#include <functional>

namespace engine::core {

class Entity_id {
public:
    using value_type = std::int64_t;

    static constexpr Entity_id invalid();
    static constexpr Entity_id any();

    constexpr Entity_id() noexcept : value_(invalid().value()) {}
    constexpr Entity_id(value_type value) noexcept : value_(value) {}

    constexpr value_type value() const noexcept { return value_; }

private:
    value_type value_;
};

constexpr Entity_id Entity_id::invalid() {
    return {-1};
}

constexpr Entity_id Entity_id::any() {
    return {-5};
}

constexpr bool operator==(const Entity_id& lhs, const Entity_id& rhs) noexcept {
    return lhs.value() == rhs.value();
}

constexpr bool operator!=(const Entity_id& lhs, const Entity_id& rhs) noexcept {
    return !(lhs == rhs);
}

} // namespace engine::core

namespace std {
template <>
struct hash<engine::core::Entity_id> {
    std::size_t operator()(const engine::core::Entity_id& entity_id) const
            noexcept {
        return std::hash<engine::core::Entity_id::value_type>{}(
                entity_id.value());
    }
};
} // namespace std

#endif /* ENGINE_CORE_ENTITY_ID_HPP */
