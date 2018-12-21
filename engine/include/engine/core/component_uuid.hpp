#ifndef ENGINE_CORE_COMPONENT_UUID_HPP
#define ENGINE_CORE_COMPONENT_UUID_HPP

#include <functional>
#include <string>
#include <string_view>

namespace engine::core {

class Component_uuid {
public:
    //! A default constructed 128 bit UUID is 0.
    constexpr Component_uuid() noexcept = default;

    //! Constructs a 128 bit UUID from two 64 bit unsigned integers.
    constexpr Component_uuid(std::uint64_t upper_half,
                             std::uint64_t lower_half) noexcept;

    //! Constructs a 128 bit UUID from a UUID string representation with the
    //! following format: ef379e48-3713-4c15-99fe-b15c4ff100e2. If the string is
    //! not a valid UUID representation, then construction still succeeds but
    //! the resulting 128 bit UUID will be 0.
    constexpr Component_uuid(std::string_view uuid) noexcept;

    constexpr std::uint64_t upper() const noexcept { return upper_half_; }
    constexpr std::uint64_t lower() const noexcept { return lower_half_; }

    std::string str() const;

    friend constexpr bool operator==(const Component_uuid&,
                                     const Component_uuid&) noexcept;

    friend constexpr bool operator!=(const Component_uuid&,
                                     const Component_uuid&) noexcept;

    friend constexpr bool operator<(const Component_uuid&,
                                    const Component_uuid&) noexcept;

    friend constexpr bool operator>(const Component_uuid&,
                                    const Component_uuid&) noexcept;

    friend constexpr bool operator<=(const Component_uuid&,
                                     const Component_uuid&) noexcept;

    friend constexpr bool operator>=(const Component_uuid&,
                                     const Component_uuid&) noexcept;

    friend struct std::hash<Component_uuid>;

private:
    constexpr bool is_hex_digit(char c) const noexcept;
    constexpr int hex_digit_value(char c) const noexcept;

    std::uint64_t upper_half_ = 0;
    std::uint64_t lower_half_ = 0;
};

constexpr Component_uuid::Component_uuid(std::uint64_t upper_half,
                                         std::uint64_t lower_half) noexcept
    : upper_half_(upper_half), lower_half_(lower_half) {}

constexpr Component_uuid::Component_uuid(std::string_view uuid) noexcept {
    if (uuid.size() != 36) {
        return;
    }

    for (std::size_t nibble_index = 0, i = 0; i < uuid.size(); ++i) {
        char c = uuid[i];

        if (c == '-' && (i == 8 || i == 13 || i == 18 || i == 23)) {
            continue;
        }

        if (!is_hex_digit(c)) {
            upper_half_ = 0;
            lower_half_ = 0;
            return;
        }

        auto hex_value = hex_digit_value(c);

        if (nibble_index < 16) {
            auto positioned_nibble = static_cast<std::uint64_t>(hex_value);
            positioned_nibble <<= 60 - 4 * nibble_index;
            upper_half_ |= positioned_nibble;
        }
        else {
            auto positioned_nibble = static_cast<std::uint64_t>(hex_value);
            positioned_nibble <<= 60 - 4 * (nibble_index - 16);
            lower_half_ |= positioned_nibble;
        }

        ++nibble_index;
    }
}

constexpr bool Component_uuid::is_hex_digit(char c) const noexcept {
    if ((c >= 0x30 && c <= 0x39) || (c >= 0x61 && c <= 0x66)) {
        return true;
    }

    return false;
}

constexpr int Component_uuid::hex_digit_value(char c) const noexcept {
    if (c >= 0x30 && c <= 0x39) {
        return c - 0x30;
    }
    else if (c >= 0x61 && c <= 0x66) {
        return c - 0x61 + 10;
    }
    else {
        return -1;
    }
}

constexpr bool operator==(const Component_uuid& lhs,
                          const Component_uuid& rhs) noexcept {
    return lhs.upper_half_ == rhs.upper_half_ &&
           lhs.lower_half_ == rhs.lower_half_;
}

constexpr bool operator!=(const Component_uuid& lhs,
                          const Component_uuid& rhs) noexcept {
    return lhs.upper_half_ != rhs.upper_half_ ||
           lhs.lower_half_ != rhs.lower_half_;
}

constexpr bool operator<(const Component_uuid& lhs,
                         const Component_uuid& rhs) noexcept {
    if (lhs.upper_half_ > rhs.upper_half_) {
        return false;
    }

    if (lhs.upper_half_ == rhs.upper_half_) {
        return lhs.lower_half_ < rhs.lower_half_;
    }

    return true;
}

constexpr bool operator>(const Component_uuid& lhs,
                         const Component_uuid& rhs) noexcept {
    if (lhs.upper_half_ < rhs.upper_half_) {
        return false;
    }

    if (lhs.upper_half_ == rhs.upper_half_) {
        return lhs.lower_half_ > rhs.lower_half_;
    }

    return true;
}

constexpr bool operator<=(const Component_uuid& lhs,
                          const Component_uuid& rhs) noexcept {
    return !(lhs > rhs);
}

constexpr bool operator>=(const Component_uuid& lhs,
                          const Component_uuid& rhs) noexcept {
    return !(lhs < rhs);
}

} // namespace engine::core

namespace std {
template <>
struct hash<engine::core::Component_uuid> {
    std::size_t operator()(const engine::core::Component_uuid& uuid) const
            noexcept {
        // TODO: Combine the hashes in a better way to avoid collisions.
        return std::hash<std::uint64_t>{}(uuid.upper_half_) ^
               std::hash<std::uint64_t>{}(uuid.lower_half_);
    }
};
} // namespace std

#endif /* ENGINE_CORE_COMPONENT_UUID_HPP */
