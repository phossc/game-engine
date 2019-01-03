#include <engine/core/component_entity.hpp>

#include <cassert>

#include <algorithm>

#include <engine/core/system.hpp>

namespace engine::core {

Component_entity::~Component_entity() {
    perform_deactivation();
}

Component* Component_entity::create(Uuid uuid) {
    // The component and its dependencies already exists.
    if (auto existing_component = get(uuid); existing_component != nullptr) {
        return existing_component;
    }

    auto dependencies = sys->component_registry().dependencies(uuid);
    if (dependencies.empty()) {
        sys->logger().warn(
                "Unable to create component from UUID {} because a component "
                "is not registered for it or one of its dependencies",
                uuid.str());
        return nullptr;
    }

    // Only move the start index of the components to be considered for
    // activation next update if no tasks were scheduled. This preserves the
    // start index value if another component creation call happened, and lets
    // deactivation and activation calls take precedence.
    if (scheduled_task_ == Task::none && active_) {
        sys->ecs().schedule_update(id());
        scheduled_task_ = Task::activate_new_components;
        new_components_start_index_ = components_.size();
    }

    for (const auto& dependency : dependencies) {
        if (components_.count(dependency) == 1) {
            continue;
        }

        auto component = sys->component_registry().create_component(dependency);
        auto raw_component = component.get();
        components_.emplace(dependency, std::move(component));
        component_creation_order_.push_back(dependency);
        raw_component->set_entity(this);

        // Last component in the dependencies is the component requested to be
        // created. All dependencies are now created, so return a pointer to the
        // requested one.
        if (dependency == uuid) {
            return raw_component;
        }
    }

    return nullptr;
}

Component* Component_entity::get(Uuid uuid) {
    auto component = components_.find(uuid);
    return component != std::end(components_) ? component->second.get()
                                              : nullptr;
}

void Component_entity::update() {
    auto task = scheduled_task_;
    scheduled_task_ = Task::none;

    switch (task) {
    case Task::none:
        break;

    case Task::deactivate:
        perform_deactivation();
        break;

    case Task::activate:
        perform_activation();
        break;

    case Task::activate_new_components:
        auto new_components_begin =
                std::next(std::begin(component_creation_order_),
                          new_components_start_index_);
        auto new_components_end = std::end(component_creation_order_);

        std::for_each(new_components_begin, new_components_end,
                      [this](const auto& component_uuid) {
                          auto component = get(component_uuid);
                          assert(component != nullptr);

                          component->activate();
                          component->active_ = true;
                      });
        break;
    }
}

void Component_entity::activate() {
    if (scheduled_task_ == Task::none) {
        sys->ecs().schedule_update(id());
    }

    scheduled_task_ = Task::activate;
}

void Component_entity::deactivate() {
    if (scheduled_task_ == Task::none) {
        sys->ecs().schedule_update(id());
    }

    scheduled_task_ = Task::deactivate;
}

void Component_entity::perform_activation() {
    std::for_each(std::begin(component_creation_order_),
                  std::end(component_creation_order_),
                  [this](const auto& component_uuid) {
                      auto component = get(component_uuid);
                      assert(component != nullptr);

                      if (!component->active()) {
                          component->activate();
                          component->active_ = true;
                      }
                  });

    active_ = true;
}

void Component_entity::perform_deactivation() {
    std::for_each(std::rbegin(component_creation_order_),
                  std::rend(component_creation_order_),
                  [this](const auto& component_uuid) {
                      auto component = get(component_uuid);
                      assert(component != nullptr);

                      if (component->active()) {
                          component->deactivate();
                          component->active_ = false;
                      }
                  });

    active_ = false;
}

} // namespace engine::core