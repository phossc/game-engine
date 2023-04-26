#ifndef ENGINE_ECS_ENTITY_BUILDER_HPP
#define ENGINE_ECS_ENTITY_BUILDER_HPP

#include "engine/ecs/component_types.hpp"
#include "engine/ecs/entity_store.hpp"

#include <algorithm>
#include <utility>
#include <vector>

namespace engine::ecs {

/// The Entity_builder helper class assists in setting up new entities. It makes
/// sure that the entity's components are ordered by their dependencies if at
/// least one of the components requires it.
template <typename EcsType>
struct Entity_builder final {
public:
    friend EcsType;

    explicit Entity_builder(EcsType& ecs) : ecs_(ecs) {}
    Entity_builder(Entity_builder&&) noexcept = default;

    /// Creates and inserts a component into its component store and passes
    /// along the provided arguments to the component's constructor.
    template <typename ComponentType, typename... Args>
    Entity_builder& with(Args&&... args) {
        auto index = ecs_.template component_store<ComponentType>().insert(
                ComponentType{std::forward<Args>(args)...});
        auto dependency_order = ecs_.template dependency_order<ComponentType>();

        components_.emplace_back(dependency_order, index.underlying_value());
        grouping_creators_.emplace_back(Grouping_creator<ComponentType>{});

        if (!sort_components_) {
            sort_components_ = Component_traits<ComponentType>::sort_deps();
        }

        return *this;
    }

    /// Schedules the entity for creation in the start of the next frame. The
    /// returned entity id will remain stable, however, it is not directly part
    /// of the ECS yet. The entity builder instance should not be used after
    /// build has been called.
    Entity_id build() { return ecs_.schedule_entity_builder(std::move(*this)); }

private:
    using Lookup_table = std::unordered_map<Uuid, Component_index>;
    using Grouping_creator_wrapper = std::function<bool(EcsType&, const Lookup_table&)>;

    template <typename ComponentType>
    class Grouping_creator {
    public:
        bool operator()(EcsType& ecs, const Lookup_table& uuid_to_index) const noexcept {
            try {
                typename ComponentType::Group_tuple grouping;
                grouping = helper(grouping, uuid_to_index);
                ecs.template component_grouping<ComponentType>().add_group(grouping);
                return true;
            } catch (...) {
                return false;
            }
        }

    private:
        template <typename... ComponentTypes>
        std::tuple<Typed_component_index<ComponentTypes>...>
        helper(const std::tuple<Typed_component_index<ComponentTypes>...>&,
               const Lookup_table& uuid_to_index) const {
            return std::make_tuple(Typed_component_index<ComponentTypes>{
                    uuid_to_index.at(Component_traits<ComponentTypes>::uuid())
                            .underlying_value()}...);
        }
    };

    /// Called by the ECS.
    Array_view<Entity_store::Entry> perform_build();

    EcsType& ecs_;

    std::vector<Entity_store::Entry> components_;
    std::vector<Grouping_creator_wrapper> grouping_creators_;

    bool sort_components_ = false;
};

template <typename EcsType>
Array_view<Entity_store::Entry> Entity_builder<EcsType>::perform_build() {
    if (sort_components_) {
        std::sort(std::begin(components_), std::end(components_),
                  [](const auto& lhs, const auto& rhs) { return lhs.first < rhs.first; });
    }

    Lookup_table uuid_to_index;
    for (const auto& entry : components_) {
        auto iter = uuid_to_index.emplace(ecs_.uuid_from(entry.first), entry.second);

        // Duplicate components in an entity.
        if (!iter.second) {
            throw std::logic_error("Components must be unique within an entity.");
        }
    }

    auto success = std::all_of(std::cbegin(grouping_creators_), std::cend(grouping_creators_),
                               [&](const auto& creator) { return creator(ecs_, uuid_to_index); });

    if (!success) {
        throw std::logic_error("Some components are missing their dependencies.");
    }

    return {components_.data(), components_.size()};
}

} // namespace engine::ecs

#endif /* ENGINE_ECS_ENTITY_BUILDER_HPP */