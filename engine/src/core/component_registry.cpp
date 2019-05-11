#include "engine/core/component_registry.hpp"

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

} // namespace engine::core
