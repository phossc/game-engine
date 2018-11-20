#ifndef ENGINE_CORE_COMPONENT_REGISTRY_HPP
#define ENGINE_CORE_COMPONENT_REGISTRY_HPP

#include <functional>
#include <map>
#include <memory>
#include <vector>

#include <engine/array_view.hpp>
#include <engine/core/component.hpp>
#include <engine/core/component_uuid.hpp>

namespace engine::core {

class Component_registry final {
public:
    //! Components must be registered before the entity component system can use
    //! them. This is required by the ECS so it can make sure that there are no
    //! missing component dependencies on entities, and so that it can
    //! instantiate components by providing a UUID.
    template <typename ComponentType>
    void register_component();

    //! Instantiates a component identified by UUID.
    //!
    //! \return an owning pointer to the created component or nullptr if a
    //! component type is not registered for the UUID.
    std::unique_ptr<Component> create_component(Component_uuid uuid);

    //! Calls create_component(ComponentType::s_uuid()).
    template <typename ComponentType>
    std::unique_ptr<ComponentType> create_component() {
        return static_cast<std::unique_ptr<ComponentType>>(
                create_component(ComponentType::s_uuid()));
    }

    //! Returns a topologically sorted list with all the dependencies of the
    //! component identified by the provided UUID including itself.
    //!
    //! If there are missing dependencies or the provided UUID does not exist
    //! then an empty list is returned.
    std::vector<Component_uuid> dependencies(Component_uuid uuid) const;

private:
    // Adds the dependent to the dependency graph if all dependencies are
    // present otherwise adds it to the incomplete dependency graph and the
    // missing dependencies are tracked. Dependents with missing dependencies
    // are resolved based on the information provided by the new dependent.
    void add_dependent(Component_uuid dependent,
                       Array_view<Component_uuid> dependencies);

    // Tracks the missing dependency by storing what dependents rely on it and
    // by storing what dependencies that a dependent relies on.
    void track_missing_dependency(Component_uuid dependency,
                                  Component_uuid dependent);

    // Recursively resolves dependencies. The provided resolved dependency and
    // the further resolved dependencies are moved from the incomplete
    // dependency graph to the complete one.
    void resolve_missing_dependencies(Component_uuid resolved_dependency);

    std::map<Component_uuid, std::function<std::unique_ptr<Component>()>>
            component_creators_;

    std::map<Component_uuid, Array_view<Component_uuid>> dependency_graph_;

    //! Contains the UUID and the dependencies of components whose dependencies
    //! are not all present in the dependency graph.
    std::map<Component_uuid, Array_view<Component_uuid>>
            incomplete_dependency_graph_;

    std::map<Component_uuid, std::vector<Component_uuid>>
            missing_dependency_to_dependents_;

    std::map<Component_uuid, std::vector<Component_uuid>>
            dependent_to_missing_dependencies_;
};

template <typename ComponentType>
void Component_registry::register_component() {
    auto uuid = ComponentType::s_uuid();
    auto dependency_uuids = ComponentType::s_dependencies();

    // Already registered.
    if (dependency_graph_.count(uuid) == 1 ||
        incomplete_dependency_graph_.count(uuid) == 1) {
        return;
    }

    add_dependent(uuid, dependency_uuids);
    component_creators_.emplace(
            uuid, []() { return std::make_unique<ComponentType>(); });
}

} // namespace engine::core

#endif /* ENGINE_CORE_COMPONENT_REGISTRY_HPP */
