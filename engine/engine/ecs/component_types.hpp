#ifndef ENGINE_ECS_COMPONENT_TYPES_HPP
#define ENGINE_ECS_COMPONENT_TYPES_HPP

#include "engine/array_view.hpp"
#include "engine/core/uuid.hpp"
#include "engine/ecs/component_traits.hpp"
#include "engine/ecs/ecs.hpp"
#include "engine/ecs/handle_types.hpp"

#include <array>
#include <tuple>

namespace engine::ecs {

struct Component_base {};

template <typename ComponentType, typename... Dependencies>
struct Component {
    using Dependencies_array = std::array<Uuid, sizeof...(Dependencies)>;
    using Group_tuple = std::tuple<Typed_component_index<ComponentType>,
                                   Typed_component_index<Dependencies>...>;

    constexpr static Uuid uuid_s() noexcept { return ComponentType::uuid; }

    constexpr static Array_view<Uuid> dependencies_s() noexcept {
        if constexpr (dependencies.empty()) {
            return {};
        } else {
            return {dependencies.data(), dependencies.size()};
        }
    }

    static constexpr Dependencies_array dependencies = [] {
        Dependencies_array deps{};
        std::size_t i = 0;
        (..., (deps[i++] = Dependencies::uuid_s()));
        return deps;
    }();
};

template <typename ComponentType, typename... Dependencies>
struct Data_component : Component<ComponentType, Dependencies...>, Component_base {
    constexpr static bool sort_dependencies() noexcept { return false; }
    constexpr static bool behavioral() noexcept { return false; }
};

class Behavior_interface : public Component_base {
public:
    virtual ~Behavior_interface() = default;
    virtual void activate() = 0;
    virtual void deactivate() = 0;

    [[nodiscard]] virtual bool active() const noexcept = 0;
    [[nodiscard]] virtual bool inactive() const noexcept = 0;

    [[nodiscard]] virtual Uuid get_uuid() const noexcept = 0;
    [[nodiscard]] virtual Array_view<Uuid> get_deps() const noexcept = 0;

protected:
    friend Ecs;
    virtual void initialize(Ecs* ecs, Component_index handle) = 0;
    virtual void set_active(bool active) noexcept = 0;
};

template <typename ComponentType, typename... Dependencies>
struct Behavior_component : Component<ComponentType, Dependencies...>, Behavior_interface {
    constexpr static bool sort_dependencies() noexcept { return true; }
    constexpr static bool behavioral() noexcept { return true; }

    void activate() override {}
    void deactivate() override {}

    [[nodiscard]] bool active() const noexcept final { return active_; }
    [[nodiscard]] bool inactive() const noexcept final { return !active_; }

    [[nodiscard]] Uuid get_uuid() const noexcept final {
        return Component_traits<ComponentType>::uuid();
    }

    [[nodiscard]] Array_view<Uuid> get_deps() const noexcept final {
        return Component_traits<ComponentType>::deps();
    }

    /// The returned dependency will outlive the component that this function is called from. This
    /// function should only be called after the component has been fully added to the ECS which
    /// happens in the beginning of the frame following entity creation. After a component has been
    /// fully added, the activate method is called.
    template <typename DependencyType>
    [[nodiscard]] DependencyType& get_dependency() const {
        assert(ecs_);
        auto group = ecs_->component_grouping<ComponentType>().get_group(handle_);
        auto dependency_handle = std::get<Typed_component_index<DependencyType>>(group);
        return ecs_->component_store<DependencyType>()[dependency_handle];
    }

private:
     void initialize(Ecs* ecs, Component_index handle) final {
        assert(!ecs_);
        ecs_ = ecs;
        handle_ = Typed_component_index<ComponentType>{handle.underlying_value()};
    }

    void set_active(bool active) noexcept final { active_ = active; }

    bool active_ = false;
    Ecs* ecs_ = nullptr;
    Typed_component_index<ComponentType> handle_;
};

} // namespace engine::ecs

#endif /* ENGINE_ECS_COMPONENT_TYPES_HPP */
