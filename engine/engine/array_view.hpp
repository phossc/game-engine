#ifndef ENGINE_ARRAY_VIEW_HPP
#define ENGINE_ARRAY_VIEW_HPP

#include <cstddef>
#include <limits>

namespace engine {

//! Non-owning view into a contiguous array of elements.
template <typename T>
class Array_view {
public:
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using iterator = T*;
    using const_iterator = const T*;
    using pointer = T*;
    using const_pointer = const T*;
    using difference_type = std::ptrdiff_t;
    using size_type = std::size_t;

    constexpr Array_view() noexcept : data_(nullptr), size_(0) {}

    constexpr Array_view(const_pointer data, size_type size) noexcept
        : data_(data), size_(size) {}

    constexpr Array_view(const Array_view<T>& src) noexcept = default;

    constexpr Array_view<T>&
    operator=(const Array_view<T>& rhs) noexcept = default;

    constexpr const_pointer data() const noexcept { return data_; }
    constexpr size_type size() const noexcept { return size_; }
    constexpr bool empty() const noexcept { return size_ == 0; }

    constexpr size_type max_size() const noexcept {
        return std::numeric_limits<size_type>::max() / sizeof(value_type);
    }

    constexpr const_iterator begin() const noexcept { return data_; }
    constexpr const_iterator end() const noexcept { return data_ + size_; }
    constexpr const_iterator cbegin() const noexcept { return data_; }
    constexpr const_iterator cend() const noexcept { return data_ + size_; }

    constexpr void swap(Array_view<T>& other) noexcept {
        auto temp_data = data_;
        auto temp_size = size_;

        data_ = other.data_;
        size_ = other.size_;

        other.data_ = temp_data;
        other.size_ = temp_size;
    }

private:
    const_pointer data_;
    size_type size_;
};

template <typename T>
constexpr bool operator==(const Array_view<T>& lhs, const Array_view<T>& rhs) {
    if (lhs.size() != rhs.size()) {
        return false;
    }

    for (int i = 0; i < lhs.size(); ++i) {
        if (*(lhs.cbegin() + i) != *(rhs.cbegin() + i)) {
            return false;
        }
    }

    return true;
}

template <typename T>
constexpr bool operator!=(const Array_view<T>& lhs, const Array_view<T>& rhs) {
    return !(lhs == rhs);
}

} // namespace engine

#endif /* ENGINE_ARRAY_VIEW_HPP */
