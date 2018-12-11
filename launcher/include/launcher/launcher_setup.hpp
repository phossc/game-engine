#ifndef LAUNCHER_LAUNCHER_SETUP_HPP
#define LAUNCHER_LAUNCHER_SETUP_HPP

namespace launcher {

class Launcher_setup final {
public:
    void register_components();
    void attach_system_components();
};

} // namespace launcher

#endif /* LAUNCHER_LAUNCHER_SETUP_HPP */