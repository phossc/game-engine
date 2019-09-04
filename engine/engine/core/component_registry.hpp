#ifndef ENGINE_CORE_COMPONENT_REGISTRY_HPP
#define ENGINE_CORE_COMPONENT_REGISTRY_HPP

#include "engine/array_view.hpp"
#include "engine/core/component.hpp"
#include "engine/core/uuid.hpp"

#include <functional>
#include <map>
#include <memory>
#include <vector>

namespace engine::core {

class Component_registry final {
public:
    //! Components must be registered before the entity component system can use
    //! them. This is required by the ECS so it can make sure that there are no
    //! missing component dependencies on entities, and so that it can
    //! instantiate components by providing a UUID. Registration of a component
    //! with the same UUID as a component previously registered will replace the
    //! registration of the old component with the new one.
    template <typename ComponentType>
    void register_component();

    //! Instantiates a component identified by UUID.
    //!
    //! \return an owning pointer to the created component or nullptr if a
    //! component type is not registered for the UUID.
    std::unique_ptr<Component> create_component(Uuid uuid);

    //! Calls create_component(ComponentType::uuid_s()).
    template <typename ComponentType>
    std::unique_ptr<ComponentType> create_component() {
        return static_cast<std::unique_ptr<ComponentType>>(
                create_component(ComponentType::uuid_s()));
    }

    //! Returns a topologically sorted list with all the dependencies of the
    //! component identified by the provided UUID including itself.
    //!
    //! If there are circular dependencies, missing dependencies or the provided
    //! UUID does not exist then an empty array is returned.
    std::vector<Uuid> dependencies(Uuid uuid) const;

private:
    std::map<Uuid, Array_view<Uuid>> dependency_graph_;
    std::map<Uuid, std::function<std::unique_ptr<Component>()>>
            component_creators_;
};

template <typename ComponentType>
void Component_registry::register_component() {
    auto dependent = ComponentType::uuid_s();
    auto dependencies = ComponentType::dependencies_s();

    // Disallow UUID 0 registration.
    if (dependent == Uuid{0, 0}) {
        return;
    }

    dependency_graph_[dependent] = dependencies;
    component_creators_[dependent] = []() {
        return std::make_unique<ComponentType>();
    };
}

} // namespace engine::core

#endif /* ENGINE_CORE_COMPONENT_REGISTRY_HPP */
