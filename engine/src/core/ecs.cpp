#include <engine/core/ecs.hpp>

namespace engine::core {

Entity* Ecs::get_entity(Entity_id entity_id) {
    auto iter = entities_.find(entity_id);
    return iter != std::end(entities_) ? iter->second.get() : nullptr;
}

void Ecs::schedule_deletion(Entity_id entity_id) {
    entities_scheduled_for_deletion_.push_back(entity_id);
}

void Ecs::schedule_update(Entity_id entity_id) {
    entities_scheduled_for_update_.push_back(entity_id);
}

void Ecs::update() {
    std::swap(entities_scheduled_for_deletion_, entities_being_deleted_);
    std::swap(entities_scheduled_for_update_, entities_being_updated_);

    for (auto id : entities_being_deleted_) {
        entities_.erase(id);
    }

    for (auto id : entities_being_updated_) {
        if (auto entity = get_entity(id); entity != nullptr) {
            entity->update();
        }
    }

    entities_being_deleted_.clear();
    entities_being_updated_.clear();
}

} // namespace engine::core
