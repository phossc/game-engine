#ifndef LAUNCHER_LAUNCHER_HPP
#define LAUNCHER_LAUNCHER_HPP

#include <engine/core/component.hpp>

namespace launcher {

class Launcher final : public engine::core::Component {
public:
    COMPONENT("57d6303e-d280-4c6a-8a32-11ee891b80db");
    DEPENDENCIES();

    //! Functions as the launcher's entry point.
    void activate() override;
    void deactivate() override;
};

} // namespace launcher

#endif /* LAUNCHER_LAUNCHER_HPP */
