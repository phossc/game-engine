#include "engine/core/game_entity.hpp"

#include <algorithm>

namespace engine::core {

Game_entity::~Game_entity() {
    for (auto child : children_) {
        child->parent_ = nullptr;
    }

    if (parent_) {
        parent_->detach(this);
    }
}

void Game_entity::attach(Game_entity* child) noexcept {
    if (child->parent_ == this) {
        return;
    }

    if (child->parent_) {
        child->parent_->detach(child);
    }

    child->parent_ = this;
    children_.push_back(child);
}

void Game_entity::detach(Game_entity* child) noexcept {
    if (child->parent_ != this) {
        return;
    }

    child->parent_ = nullptr;
    auto iter = std::find(std::begin(children_), std::end(children_), child);
    std::swap(children_.back(), *iter);
    children_.pop_back();
}

Game_entity* Game_entity::parent() const noexcept {
    if (!parent_) {
        return nullptr;
    }

    if (!parent_->active()) {
        return nullptr;
    }

    return parent_;
}

} // namespace engine::core