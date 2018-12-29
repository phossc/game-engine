#include <engine/core/component_registry.hpp>

#include <algorithm>
#include <stack>
#include <unordered_map>

namespace engine::core {

std::unique_ptr<engine::core::Component>
Component_registry::create_component(Uuid uuid) {
    auto iter = component_creators_.find(uuid);
    if (iter != std::end(component_creators_)) {
        return iter->second();
    }

    return nullptr;
}

std::vector<Uuid> Component_registry::dependencies(Uuid uuid) const {
    // A component can't be registered for a uuid with value 0.
    if (uuid == Uuid(0, 0)) {
        return {};
    }

    enum class Node_status { discovered, finished };
    std::unordered_map<Uuid, Node_status> node_statuses;
    std::stack<Uuid> nodes;
    std::vector<Uuid> sorted_dependencies;

    nodes.push(uuid);
    node_statuses.emplace(uuid, Node_status::discovered);

    while (!nodes.empty()) {
        auto node = nodes.top();

        // The uuid with the value 0 is used to separate nodes at different
        // depths and can't be at the bottom of the nodes stack.
        if (node == Uuid(0, 0)) {
            nodes.pop();
            auto finished_node = nodes.top();
            node_statuses[finished_node] = Node_status::finished;
            sorted_dependencies.push_back(finished_node);
            nodes.pop();
            continue;
        }

        if (auto dependencies = dependency_graph_.find(node);
            dependencies == std::end(dependency_graph_)) {
            return {};
        }
        else if (dependencies->second.empty()) {
            node_statuses[node] = Node_status::finished;
            sorted_dependencies.push_back(node);
            nodes.pop();
        }
        else {
            // Push graph depth separator.
            nodes.emplace(0, 0);

            for (const auto& dependency : dependencies->second) {
                auto inserted = node_statuses.emplace(dependency,
                                                      Node_status::discovered);

                if (inserted.second) {
                    nodes.push(dependency);
                }
                // Circular dependency.
                else if (!inserted.second &&
                         inserted.first->second == Node_status::discovered) {
                    return {};
                }
            }
        }
    }

    return sorted_dependencies;
}

void Component_registry::add_dependent(Uuid dependent,
                                       Array_view<Uuid> dependencies) {
    bool missing_dependencies = false;
    for (auto dependency : dependencies) {
        if (dependency_graph_.count(dependency) == 0) {
            missing_dependencies = true;
            track_missing_dependency(dependency, dependent);
        }
    }

    incomplete_dependency_graph_.emplace(dependent, dependencies);

    if (!missing_dependencies) {
        resolve_missing_dependencies(dependent);
    }
}

void Component_registry::track_missing_dependency(Uuid dependency,
                                                  Uuid dependent) {
    if (auto iter = missing_dependency_to_dependents_.find(dependency);
        iter != std::end(missing_dependency_to_dependents_)) {
        iter->second.push_back(dependent);
    }
    else {
        std::vector<Uuid> dependents = {dependent};
        missing_dependency_to_dependents_.emplace(dependency,
                                                  std::move(dependents));
    }

    if (auto iter = dependent_to_missing_dependencies_.find(dependent);
        iter != std::end(dependent_to_missing_dependencies_)) {
        iter->second.push_back(dependency);
    }
    else {
        std::vector<Uuid> dependencies = {dependency};
        dependent_to_missing_dependencies_.emplace(dependent,
                                                   std::move(dependencies));
    }
}

void Component_registry::resolve_missing_dependencies(
        Uuid resolved_dependency) {
    // Dependencies always start in the incomplete dependency graph. Move the
    // resolved dependency from the incomplete to the complete dependency graph.
    auto dependency_transfer =
            incomplete_dependency_graph_.find(resolved_dependency);
    dependency_graph_.emplace(dependency_transfer->first,
                              dependency_transfer->second);
    incomplete_dependency_graph_.erase(dependency_transfer);

    auto dependents =
            missing_dependency_to_dependents_.find(resolved_dependency);

    // There are no dependents on the resolved dependency so no more
    // dependencies can be resolved.
    if (dependents == std::end(missing_dependency_to_dependents_)) {
        return;
    }

    std::vector<Uuid> resolved_dependencies;
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
            std::swap(*dependency, dependencies->second.back());
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
