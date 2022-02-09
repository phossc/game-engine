#ifndef ENGINE_ECS_COMPONENT_STORE_HPP
#define ENGINE_ECS_COMPONENT_STORE_HPP

#include "engine/ecs/handle_types.hpp"

#include <deque>
#include <optional>
#include <vector>

namespace engine::ecs {

struct Component_store_base {};

/// The Component_store class is responsible for storing components of the same
/// type. An inserted component is referenced by an index that remain stable
/// until that component is removed. References to stored components are valid
/// for at least one frame.
template <typename ComponentType>
class Component_store final : public Component_store_base {
public:
    /// Inserts a component into the component store and returns an index to
    /// that component that can be used to reference that component until the
    /// component is removed. Does not invalidate references.
    Typed_component_index<ComponentType> insert(ComponentType component) {
        if (available_indicies_.empty()) {
            components_.push_back(std::move(component));
            return Typed_component_index<ComponentType>{components_.size() - 1};
        }
        else {
            auto index = available_indicies_.back();
            components_[index] = std::move(component);
            available_indicies_.pop_back();
            return Typed_component_index<ComponentType>{index};
        }
    }

    /// Removes the component at the specified index. Does nothing if there is
    /// no component at that index. Does not invalidate references.
    void remove(Typed_component_index<ComponentType> index) {
        auto i = index.underlying_value();
        if (i >= 0 && i < components_.size() && components_[i].has_value()) {
            components_[i].reset();
            available_indicies_.push_back(i);
        }
    }

    /// Generic component index version of remove.
    void remove(Component_index index) {
        remove(Typed_component_index<ComponentType>{index.underlying_value()});
    }

    /// Returns a reference to the component at the given index.
    /// The behavior is undefined if the index is invalid.
    ComponentType&
    operator[](Typed_component_index<ComponentType> index) noexcept {
        return *components_[index.underlying_value()];
    }

    /// Returns a read-only reference to the component at the given index.
    /// The behavior is undefined if the index is invalid.
    const ComponentType&
    operator[](Typed_component_index<ComponentType> index) const noexcept {
        return *components_[index.underlying_value()];
    }

    /// Generic component index version of the subscript operator.
    ComponentType& operator[](Component_index index) noexcept {
        return *components_[index.underlying_value()];
    }

    /// Generic component index version of the const subscript operator.
    const ComponentType& operator[](Component_index index) const noexcept {
        return *components_[index.underlying_value()];
    }

private:
    std::deque<std::optional<ComponentType>> components_;
    std::vector<typename Typed_component_index<
            ComponentType>::underlying_value_type>
            available_indicies_;
};

} // namespace engine::ecs

#endif /* ENGINE_ECS_COMPONENT_STORE_HPP */