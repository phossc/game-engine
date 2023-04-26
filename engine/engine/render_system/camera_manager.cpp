#include "engine/render_system/camera_manager.hpp"

#include "engine/render_system/camera.hpp"

namespace engine {

void Camera_manager::set_active_camera(Camera* camera) noexcept {
    if (cameras_.count(camera) == 0) {
        return;
    }

    active_camera_ = camera;
}

void Camera_manager::add_camera(Camera* camera) noexcept {
    cameras_.insert(camera);
}

void Camera_manager::remove_camera(Camera* camera) noexcept {
    cameras_.erase(camera);
    if (camera == active_camera_) {
        auto new_camera = std::begin(cameras_);
        active_camera_ = new_camera == std::end(cameras_) ? nullptr : *new_camera;
    }
}

} // namespace engine