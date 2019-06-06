#ifndef ENGINE_RENDER_SYSTEM_CAMERA_MANAGER_HPP
#define ENGINE_RENDER_SYSTEM_CAMERA_MANAGER_HPP

#include "engine/core/component.hpp"

#include <set>

namespace engine {

class Camera;

class Camera_manager final : public core::Component {
public:
    COMPONENT("b5f78703-1376-4423-8322-3cf3a2583bb3");
    DEPENDENCIES();

    //! Returns the active camera if one is set.
    Camera* active_camera() const noexcept { return active_camera_; }

    //! Sets the active camera. The camera must be added to the camera manager
    //! before it can be set as the active one.
    void set_active_camera(Camera* camera) noexcept;

    //! Adds a camera to the camera manager if not present already.
    void add_camera(Camera* camera) noexcept;

    //! Removes a camera from the camera manager.
    //!
    //! If the removed camera is the active one, then a random camera is set as
    //! the active camera if available.
    void remove_camera(Camera* camera) noexcept;

private:
    Camera* active_camera_ = nullptr;
    std::set<Camera*> cameras_;
};

} // namespace engine

#endif /* ENGINE_RENDER_SYSTEM_CAMERA_MANAGER_HPP */