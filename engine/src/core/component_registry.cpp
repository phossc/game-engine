#include <engine/core/component_registry.hpp>

#include <algorithm>

namespace engine::core {

void Component_registry::add_dependent(
        Component_uuid dependent, Array_view<Component_uuid> dependencies) {
    bool missing_dependencies = false;
    for (auto dependency : dependencies) {
        if (dependency_graph_.count(dependency) == 0) {
            missing_dependencies = true;
            track_missing_dependency(dependency, dependent);
        }
    }

    incomplete_dependency_graph_.emplace({dependent, dependencies});

    if (!missing_dependencies) {
        resolve_missing_dependencies(dependent);
    }
}

void Component_registry::track_missing_dependency(Component_uuid dependency,
                                                  Component_uuid dependent) {
    if (auto iter = missing_dependency_to_dependents_.find(dependency);
        iter != std::end(missing_dependency_to_dependents_)) {
        iter->second.push_back(dependent);
    }
    else {
        missing_dependency_to_dependents_.emplace({dependency, {dependent}});
    }

    if (auto iter = dependent_to_missing_dependencies_.find(dependent);
        iter != std::end(dependent_to_missing_dependencies_)) {
        iter->second.push_back(dependency);
    }
    else {
        dependent_to_missing_dependencies_.emplace({dependent, {dependency}});
    }
}

void Component_registry::resolve_missing_dependencies(
        Component_uuid resolved_dependency) {
    // Dependencies always start in the incomplete dependency graph. Move the
    // resolved dependency from the incomplete to the complete dependency graph.
    auto dependency_transfer =
            incomplete_dependency_graph_.find(resolved_dependency);
    dependency_graph_.emplace(
            {dependency_transfer->first, dependency_transfer->second});
    incomplete_dependency_graph_.erase(dependency_transfer);

    auto dependents =
            missing_dependency_to_dependents_.find(resolved_dependency);

    // There are no dependents on the resolved dependency so no more
    // dependencies can be resolved.
    if (dependents == std::end(missing_dependency_to_dependents_)) {
        return;
    }

    std::vector<Component_uuid> resolved_dependencies;
    for (auto dependent : dependents->second) {
        // Guaranteed to exist.
        auto dependencies = dependent_to_missing_dependencies_.find(dependent);

        // The only dependency is the resolved one which makes the dependent
        // resolved as well.
        if (dependencies->second.size() == 1) {
            resolved_dependencies.push_back(dependent);
        }
        // The dependent depends on more missing dependencies than the resolved
        // one. Erase the resolved dependency from the missing dependencies as
        // that one is no longer missing.
        else {
            auto dependency = std::find(std::begin(dependencies->second),
                                        std::end(dependencies->second),
                                        resolved_dependency);
            std::swap(*dependency, dependencies->second.last());
            dependencies->second.pop_back();
        }
    }

    missing_dependency_to_dependents_.erase(dependents);

    for (auto uuid : resolved_dependencies) {
        dependent_to_missing_dependencies_.erase(uuid);
    }

    for (auto uuid : resolved_dependencies) {
        resolve_missing_dependencies(uuid);
    }
}

} // namespace engine::core
