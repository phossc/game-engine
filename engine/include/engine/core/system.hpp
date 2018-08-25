#ifndef ENGINE_CORE_SYSTEM_HPP
#define ENGINE_CORE_SYSTEM_HPP

#include <memory>
#include <string>
#include <vector>

namespace engine::core {

class System final {
public:
    System(int argc, const char** argv);

    void run();
    void register_components();
    void attach_system_components();

    const std::string& name() { return name_; }
    const std::vector<std::string>& arguments() { return arguments_; }

private:
    std::string name_;
    std::vector<std::string> arguments_;
};

extern std::unique_ptr<System> sys;

} // namespace engine::core

#endif /* ENGINE_CORE_SYSTEM_HPP */
