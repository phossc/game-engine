#ifndef ENGINE_RENDER_SYSTEM_CAMERA_HPP
#define ENGINE_RENDER_SYSTEM_CAMERA_HPP

#include "engine/render_system/frustum.hpp"

#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"

namespace engine {

class Camera {
public:
    virtual ~Camera() = default;

    double yaw() { return yaw_; }
    void set_yaw(double yaw) {
        yaw_ = yaw;
        orientation_needs_update_ = true;
        view_matrix_needs_update_ = true;
    }

    double pitch() { return pitch_; }
    void set_pitch(double pitch) {
        pitch_ = pitch;
        orientation_needs_update_ = true;
        view_matrix_needs_update_ = true;
    }

    glm::dvec3 position() { return position_; }
    void set_position(const glm::dvec3& position) {
        position_ = position;
        view_matrix_needs_update_ = true;
    }

    glm::dvec3 forward_dir() const noexcept { return orientation()[2]; }

    glm::dvec3 up_dir() const noexcept { return orientation()[1]; }

    const glm::dmat4& orientation() const;
    const glm::dmat4& view_matrix() const;

    Frustum frustum;

protected:
    double yaw_{0};
    double pitch_{0};
    glm::dvec3 position_{0, 0, 0};

    mutable glm::dmat4 orientation_;
    mutable bool orientation_needs_update_ = true;

    mutable glm::dmat4 view_matrix_;
    mutable bool view_matrix_needs_update_ = true;
};

} // namespace engine

#endif /* ENGINE_RENDER_SYSTEM_CAMERA_HPP */