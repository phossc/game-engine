#ifndef ENGINE_ECS_COMPONENT_GROUPING_HPP
#define ENGINE_ECS_COMPONENT_GROUPING_HPP

#include "engine/ecs/component_store.hpp"

#include <cstddef>
#include <functional>
#include <iterator>
#include <stdexcept>
#include <tuple>
#include <unordered_map>
#include <vector>

namespace engine::ecs {

/// The Component_grouping class stores groups of component store indicies. A
/// component group for a given component type consists of the index of that
/// component and the indicies of its direct dependencies. Each group
/// corresponds to an entity that contains those components. The class also
/// provides iteration through all entities that contain this type of component.
/// Iterating through only a subset of the components in a group is possible
/// i.e. if a component, C, has 2 dependencies D1 and D2, including only D1 in
/// the iteration is possible. Doing that would iterate through all D1
/// components that also has the component C in the entity.
template <typename ComponentType, typename StoreProvider>
class Component_grouping final {
private:
    using Group_storage = std::vector<typename ComponentType::Group_tuple>;

public:
    /// The Iter class is an iterator capable of iterating through all groups in
    /// the component grouping providing a tuple of pointers to the components
    /// in the component stores. The template parameter ...Members is used to
    /// specify which component pointers to include in the tuple i.e. all of
    /// the components in the group or a subset thereof. A tuple of pointers was
    /// chosen over a tuple of references since it's semantics are more clear
    /// when used with structured bindings.
    template <typename... Members>
    class Iter {
        static_assert(sizeof...(Members) >= 1);
        static_assert(sizeof...(Members) <=
                      std::tuple_size_v<typename ComponentType::Group_tuple>);

    public:
        using difference_type = typename std::iterator_traits<
                typename Group_storage::iterator>::difference_type;
        using reference = std::tuple<Members*...>;
        using value_type = reference;
        using pointer = value_type*;
        using iterator_category = std::input_iterator_tag;

        class Member_of_pointer_proxy {
        public:
            Member_of_pointer_proxy(value_type value)
                : value_(std::move(value)) {}
            pointer operator->() noexcept { return &value_; }

        private:
            value_type value_;
        };

        class Post_increment_proxy {
        public:
            Post_increment_proxy(value_type value) : value_(std::move(value)) {}
            value_type operator*() noexcept { return value_; }

        private:
            value_type value_;
        };

        Iter(typename Group_storage::iterator iter,
             Component_store<Members>&... stores)
            : iter_(iter), stores_(std::ref(stores)...) {}

        /// Looks up the requested components in their component stores. The
        /// component indicies used to look up components are retrieved from the
        /// current group tuple pointed to by the iterator.
        reference operator*() noexcept {
            return std::make_tuple(
                    &std::get<std::reference_wrapper<Component_store<Members>>>(
                             stores_)
                             .get()[std::get<Typed_component_index<Members>>(
                                     *iter_)]...);
        }

        Member_of_pointer_proxy operator->() noexcept { return {operator*()}; }

        Iter<Members...>& operator++() noexcept {
            ++iter_;
            return *this;
        }

        Post_increment_proxy operator++(int) noexcept {
            Post_increment_proxy value{operator*()};
            operator++();
            return value;
        }

        /// Only compares the underlying iter_ as there is no way for stores_ to
        /// be different for iterators of the same type because there is only
        /// one of each component store.
        bool operator==(const Iter<Members...>& rhs) const {
            return iter_ == rhs.iter_;
        }

        /// Only compares the underlying iter_ as there is no way for stores_ to
        /// be different for iterators of the same type because there is only
        /// one of each component store.
        bool operator!=(const Iter<Members...>& rhs) const {
            return iter_ != rhs.iter_;
        }

    private:
        typename Group_storage::iterator iter_;
        std::tuple<std::reference_wrapper<Component_store<Members>>...> stores_;
    };

    /// The View class simplifies the construction of iterators.
    template <typename... Members>
    class View {
    public:
        View(StoreProvider& provider, typename Group_storage::iterator first,
             typename Group_storage::iterator last)
            : provider_(provider), first_(first), last_(last) {}

        Iter<Members...> begin() noexcept {
            return Iter<Members...>(
                    first_, provider_.template component_store<Members>()...);
        }

        Iter<Members...> end() noexcept {
            return Iter<Members...>(
                    last_, provider_.template component_store<Members>()...);
        }

    private:
        StoreProvider& provider_;
        typename Group_storage::iterator first_;
        typename Group_storage::iterator last_;
    };

    Component_grouping(StoreProvider& store_provider)
        : store_provider_(store_provider) {}

    /// Invalidates every view and iterator.
    /// Currently no exception guarantee.
    /// A group is identified by the Typed_component_index<ComponentType> value
    /// in it. Having two groups with the same identifier stored at the same
    /// time is not supported and one of them must be removed before the other
    /// can be added.
    void add_group(typename ComponentType::Group_tuple group) {
        auto stable_index =
                std::get<Typed_component_index<ComponentType>>(group);
        if (index_lookup_table_.count(stable_index) == 1) {
            throw std::logic_error("Multiple groups with the same identifier "
                                   "is not supported.");
        }

        groups_.push_back(std::move(group));
        index_lookup_table_[stable_index] = groups_.size() - 1;
    }

    /// Invalidates all views. Some iterators may be invalidated too.
    /// Currently no exception guarantee.
    /// Trying to remove a group that is not stored results in an exception
    /// thrown.
    void remove_group(Typed_component_index<ComponentType> stable_index) {
        // Index is a valid index into groups_.
        auto index = index_lookup_table_.at(stable_index);

        // Not the last group.
        if (index < groups_.size() - 1) {
            std::swap(groups_[index], groups_.back());
            auto stable_index_relocated =
                    std::get<Typed_component_index<ComponentType>>(
                            groups_[index]);
            index_lookup_table_[stable_index_relocated] = index;
        }

        groups_.pop_back();
        index_lookup_table_.erase(stable_index);
    }

    template <typename... Members>
    View<Members...> view() noexcept {
        return View<Members...>(store_provider_, std::begin(groups_),
                                std::end(groups_));
    }

private:
    Group_storage groups_;
    std::unordered_map<Typed_component_index<ComponentType>, std::size_t>
            index_lookup_table_;

    StoreProvider& store_provider_;
};

} // namespace engine::ecs

#endif /* ENGINE_ECS_COMPONENT_GROUPING_HPP */