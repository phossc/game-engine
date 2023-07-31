#include "engine/ecs/ecs.hpp"

#include "engine/ecs/component_types.hpp"

#include <algorithm>
#include <cassert>
#include <unordered_map>
#include <vector>

namespace engine::ecs {

void Ecs::register_components(
        const std::function<void(const Ecs::Component_registrar&)>& registration_function) {
    if (registration_done_) {
        throw std::logic_error("Component registration can only happen once.");
    }

    registration_function(component_registrar_);

    bool success = calculate_dependency_ordering();
    if (!success) {
        throw std::logic_error("Missing or circular dependencies in the components.");
    }

    registration_done_ = true;
}

void Ecs::lifecycle_update() {
    build_scheduled_entities();
    delete_scheduled_entities();
}

Entity_id Ecs::schedule_entity_builder(Entity_builder<Ecs> builder) {
    auto entity_id = current_entity_id_++;
    scheduled_entity_builders_.emplace_back(entity_id, std::move(builder));
    return entity_id;
}

bool Ecs::calculate_dependency_ordering() {
    enum class Vertex_status { undiscovered, discovered, finished };

    std::unordered_map<Uuid, Vertex_status> status;
    std::vector<Uuid> processing;

    auto handle_undiscovered = [&](const auto& vertex) {
        auto iter = dependency_graph_.find(vertex);

        // Vertex is not part of the dependency graph.
        if (iter == std::end(dependency_graph_)) {
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
            } else if (result.first->second == Vertex_status::discovered) {
                // Circular dependency.
                return false;
            }
        }

        return true;
    };

    Dependency_order dependency_order{0};
    for (const auto& [uuid, dependencies] : dependency_graph_) {
        if (auto iter = status.emplace(uuid, Vertex_status::undiscovered);
            iter.first->second == Vertex_status::undiscovered) {
            processing.push_back(uuid);
        } else {
            continue;
        }

        while (!processing.empty()) {
            Uuid current_vertex = processing.back();
            auto status_iter = status.find(current_vertex); // Always valid
            switch (status_iter->second) {
            case Vertex_status::undiscovered:
                if (!handle_undiscovered(current_vertex)) {
                    return false;
                }
                break;

            case Vertex_status::discovered:
                processing.pop_back();
                status_iter->second = Vertex_status::finished;
                dependency_ordering_.emplace(current_vertex, dependency_order);
                dependency_order_to_uuid_.emplace(dependency_order, current_vertex);
                dependency_order_to_behavioral_.emplace(
                        dependency_order, component_to_behavioral_.find(current_vertex)->second);
                ++dependency_order;
                break;

            case Vertex_status::finished:
                assert(false);
            }
        }
    }

    return true;
}

void Ecs::activate_if_behavior_component(const Entity_store::Entry& entry) {
    auto order = entry.first;
    auto handle = entry.second;
    auto is_behavioral = dependency_order_to_behavioral_.find(order)->second;
    if (is_behavioral) {
        auto& store = *component_stores_.find(uuid_from(order))->second;
        auto& behavior_component = static_cast<Behavior_interface&>(store.get(handle));
        behavior_component.initialize(this, handle);
        behavior_component.activate();
        behavior_component.set_active(true);
    }
}

void Ecs::deactivate_if_behavior_component(const Entity_store::Entry& entry) {
    auto order = entry.first;
    auto handle = entry.second;
    auto is_behavioral = dependency_order_to_behavioral_.find(order)->second;
    if (is_behavioral) {
        auto& store = *component_stores_.find(uuid_from(order))->second;
        auto& behavior_component = static_cast<Behavior_interface&>(store.get(handle));
        behavior_component.deactivate();
        behavior_component.set_active(false);
    }
}

void Ecs::build_scheduled_entities() {
    for (auto& builder_entry : scheduled_entity_builders_) {
        auto components = builder_entry.second.perform_build();
        auto range = entity_store_.create(components);
        entities_.emplace(builder_entry.first, range);

        auto inserted_components = entity_store_.get_entity_components(range);
        std::for_each(std::cbegin(inserted_components), std::cend(inserted_components),
                      [this](const auto& entry) { activate_if_behavior_component(entry); });
    }

    scheduled_entity_builders_.clear();
}

void Ecs::delete_scheduled_entities() {
    for (auto id : entities_to_delete_) {
        if (const auto iter = entities_.find(id); iter != std::cend(entities_)) {
            auto components = entity_store_.get_entity_components(iter->second);

            std::for_each(std::crbegin(components), std::crend(components),
                          [this](const auto& entry) { deactivate_if_behavior_component(entry); });

            std::for_each(std::crbegin(components), std::crend(components),
                          [this](const auto& entry) {
                              const auto uuid = uuid_from(entry.first);
                              const auto handle = entry.second;
                              component_groupings_.at(uuid)->remove_group(handle);
                              component_stores_.at(uuid)->remove(handle);
                          });

            entity_store_.erase(iter->second);
        }
    }

    entities_to_delete_.clear();
}

} // namespace engine::ecs
