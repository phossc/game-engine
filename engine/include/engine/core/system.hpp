#ifndef ENGINE_CORE_SYSTEM_HPP
#define ENGINE_CORE_SYSTEM_HPP

#include <memory>
#include <string>
#include <vector>

#include <engine/core/component_registry.hpp>
#include <engine/core/ecs.hpp>

namespace engine::core {

class System final {
public:
    System(int argc, const char** argv);

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

    Ecs& ecs() noexcept { return ecs_; }

    class System_entity& entity() const noexcept {
        return *entity_;
    }

private:
    std::string name_;
    std::vector<std::string> arguments_;

    Component_registry component_registry_;
    Ecs ecs_;
    class System_entity* entity_ = nullptr;
};

extern std::unique_ptr<System> sys;

} // namespace engine::core

#endif /* ENGINE_CORE_SYSTEM_HPP */
