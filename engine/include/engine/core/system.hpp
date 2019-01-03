#ifndef ENGINE_CORE_SYSTEM_HPP
#define ENGINE_CORE_SYSTEM_HPP

#include <memory>
#include <string>
#include <vector>

#include <engine/core/clock.hpp>
#include <engine/core/component_registry.hpp>
#include <engine/core/ecs.hpp>
#include <engine/core/game_loop.hpp>
#include <engine/core/system_entity.hpp>
#include <engine/core/update_system.hpp>
#include <spdlog/logger.h>

namespace engine::core {

class System final {
public:
    System(int argc, const char** argv);
    ~System();

    void run();
    void register_components();
    void attach_system_components();

    const std::string& name() const noexcept { return name_; }

    const std::vector<std::string>& arguments() const noexcept {
        return arguments_;
    }

    Component_registry& component_registry() noexcept {
        return component_registry_;
    }

    // References should not be used outside the lifetime of this instance.
    spdlog::logger& logger() noexcept { return *logger_; }

    Ecs& ecs() noexcept { return ecs_; }

    Clock& game_clock() noexcept { return game_clock_; }

    Update_system& update_system() noexcept { return update_system_; }

    Game_loop& game_loop() noexcept { return game_loop_; }

    System_entity* entity() const noexcept { return entity_; }

    //! Returns a component from the system entity.
    Component* component(Uuid uuid) { return entity_->get(uuid); }

    //! Returns a component from the system entity.
    template <typename ComponentType>
    ComponentType* component() {
        return entity_->get<ComponentType>();
    }

private:
    std::string name_;
    std::vector<std::string> arguments_;

    std::shared_ptr<spdlog::logger> logger_;
    Component_registry component_registry_;
    Ecs ecs_;
    Clock game_clock_;
    Update_system update_system_;
    Game_loop game_loop_;

    System_entity* entity_ = nullptr;
};

extern System* sys;

} // namespace engine::core

#endif /* ENGINE_CORE_SYSTEM_HPP */
