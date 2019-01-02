#ifndef RENDER_SYSTEM_CAMERA_HPP
#define RENDER_SYSTEM_CAMERA_HPP

#include <glm/ext/scalar_constants.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <render_system/frustum.hpp>

class Camera {
public:
    Camera() { update_view_matrix(); }
    virtual ~Camera() = default;

    void set_yaw(double yaw);
    void set_pitch(double pitch);
    void set_position(const glm::dvec3& position);

    const glm::dmat4& view_matrix() const { return view_matrix_; }
    const glm::dvec3& position() { return position_; }

    double yaw() { return yaw_; }
    double pitch() { return pitch_; }

    Frustum frustum;

protected:
    // pitch coordinates for center target
    double yaw_{0};                       // yaw
    double pitch_{glm::pi<double>() / 2}; // pitch

    glm::dvec3 position_{0, 0, 0};

    virtual void update_view_matrix();

    glm::dmat4 view_matrix_;
};

#endif /* RENDER_SYSTEM_CAMERA_HPP */