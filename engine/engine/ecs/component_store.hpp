#ifndef ENGINE_ECS_COMPONENT_STORE_HPP
#define ENGINE_ECS_COMPONENT_STORE_HPP

#include "jss/strong_typedef.hpp"

#include <cstddef>
#include <deque>
#include <optional>
#include <vector>

namespace engine::ecs {

/// Defined outside the Component_store class so that the store index can be
/// used where only an incomplete component type is available.
template <typename ComponentType>
using Store_index =
        jss::strong_typedef<ComponentType, std::size_t,
                            jss::strong_typedef_properties::comparable,
                            jss::strong_typedef_properties::hashable,
                            jss::strong_typedef_properties::streamable>;

/// The Component_store class is responsible for storing components of the same
/// type. An inserted component is referenced by an index that remain stable
/// until that component is removed. Although indicies remain stable, the
/// components themselves might be moved around as the internal storage needs
/// resizing. References to stored components are valid for at least one frame.
template <typename ComponentType>
class Component_store final {
public:
    /// The Component_store class implementation requires that the component
    /// type is complete, therefore this index type can only be used where the
    /// component type is complete. If this index type is required where only an
    /// incomplete component type is available, use Store_index instead.
    using Index = Store_index<ComponentType>;

    /// Inserts a component into the component store and returns an index to
    /// that component that can be used to reference that component until the
    /// component is removed.
    Index insert(ComponentType component) {
        if (available_indicies_.empty()) {
            components_.push_back(std::move(component));
            return Index{components_.size() - 1};
        }
        else {
            auto index = available_indicies_.back();
            components_[index] = std::move(component);
            available_indicies_.pop_back();
            return Index{index};
        }
    };

    /// Removes the component at the specified index. Does nothing if there is
    /// no component at that index.
    void remove(Index index) {
        auto i = index.underlying_value();
        if (i >= 0 && i < components_.size() && components_[i].has_value()) {
            components_[i].reset();
            available_indicies_.push_back(i);
        }
    }

    /// Returns a reference to the component at the given index.
    /// The behavior is undefined if the index is invalid.
    ComponentType& operator[](Index index) noexcept {
        return *components_[index.underlying_value()];
    }

    /// Returns a read-only reference to the component at the given index.
    /// The behavior is undefined if the index is invalid.
    const ComponentType& operator[](Index index) const noexcept {
        return *components_[index.underlying_value()];
    }

private:
    std::deque<std::optional<ComponentType>> components_;
    std::vector<typename Index::underlying_value_type> available_indicies_;
};

} // namespace engine::ecs

#endif /* ENGINE_ECS_COMPONENT_STORE_HPP */