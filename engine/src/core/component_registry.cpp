#include "engine/core/component_registry.hpp"

#include <unordered_map>
#include <vector>

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
    enum class Vertex_status { undiscovered, discovered, finished };
    std::unordered_map<Uuid, Vertex_status> status;
    std::vector<Uuid> processing;
    std::vector<Uuid> sorted;

    auto handle_undiscovered = [&](const auto& vertex) {
        auto iter = dependency_graph_.find(vertex);
        if (iter == std::end(dependency_graph_)) {
            // Vertex is not part of the dependency graph.
            return false;
        }

        status[vertex] = Vertex_status::discovered;
        const auto& dependencies = iter->second;

        auto first = std::begin(dependencies);
        auto last = std::end(dependencies);
        for (; first != last; ++first) {
            auto result = status.emplace(*first, Vertex_status::undiscovered);
            if (result.first->second == Vertex_status::undiscovered) {
                processing.push_back(*first);
            }
            else if (result.first->second == Vertex_status::discovered) {
                // Circular dependency.
                return false;
            }
        }

        return true;
    };

    processing.push_back(uuid);
    status[uuid] = Vertex_status::undiscovered;
    while (!processing.empty()) {
        Uuid current_vertex = processing.back();
        auto status_iter = status.find(current_vertex); // Always valid
        switch (status_iter->second) {
        case Vertex_status::undiscovered:
            if (!handle_undiscovered(current_vertex)) {
                sorted.clear();
                return sorted;
            }
            break;

        case Vertex_status::discovered:
            processing.pop_back();
            status_iter->second = Vertex_status::finished;
            sorted.push_back(current_vertex);
            break;

        case Vertex_status::finished:
            processing.pop_back();
            break;
        }
    }

    return sorted;
}

} // namespace engine::core
