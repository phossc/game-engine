#include <engine/core/update_system.hpp>

namespace engine::core {

using Updatables = std::multimap<std::int32_t, Updatable*>;
using Updatables_iterators =
        std::unordered_map<Updatable*, Updatables::iterator>;

static void register_update(Updatable* updatable, std::int32_t priority,
                            Updatables& updatables,
                            Updatables_iterators& iterators) {
    // Updatable has already been registered.
    if (iterators.count(updatable) > 0) {
        return;
    }

    auto iter = updatables.emplace(priority, updatable);
    iterators.emplace(updatable, iter);
}

static void deregister_update(Updatable* updatable, Updatables& updatables,
                              Updatables_iterators& iterators) {
    auto iter = iterators.find(updatable);

    // Updatable is not registered.
    if (iter == std::end(iterators)) {
        return;
    }

    updatables.erase(iter->second);
    iterators.erase(iter);
}

void Update_system::broadcast_variable_update(double dt) {
    for (auto& value : variable_updatables_) {
        value.second->variable_update(dt);
    }
}

void Update_system::broadcast_interpolation_update(double a) {
    for (auto& value : interpolation_updatables_) {
        value.second->interpolation_update(a);
    }
}

void Update_system::broadcast_fixed_update(double dt) {
    for (auto& value : fixed_updatables_) {
        value.second->fixed_update(dt);
    }
}

void Update_system::register_variable_update(Updatable* updatable,
                                             std::int32_t priority) {
    register_update(updatable, priority, variable_updatables_,
                    iterators_to_variable_updatables_);
}

void Update_system::deregister_variable_update(Updatable* updatable) {
    deregister_update(updatable, variable_updatables_,
                      iterators_to_variable_updatables_);
}

void Update_system::register_interpolation_update(Updatable* updatable,
                                                  std::int32_t priority) {
    register_update(updatable, priority, interpolation_updatables_,
                    iterators_to_interpolation_updatables_);
}

void Update_system::deregister_interpolation_update(Updatable* updatable) {
    deregister_update(updatable, interpolation_updatables_,
                      iterators_to_interpolation_updatables_);
}

void Update_system::register_fixed_update(Updatable* updatable,
                                          std::int32_t priority) {
    register_update(updatable, priority, fixed_updatables_,
                    iterators_to_fixed_updatables_);
}

void Update_system::deregister_fixed_update(Updatable* updatable) {
    deregister_update(updatable, fixed_updatables_,
                      iterators_to_fixed_updatables_);
}

} // namespace engine::core