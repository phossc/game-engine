#include "camera.hpp"

#include <cmath>

#include <glm/ext/matrix_transform.hpp>

void Camera::set_yaw(double yaw) {
    yaw_ = std::fmod(yaw, glm::pi<double>() * 2.0);
    update_view_matrix();
}

void Camera::set_pitch(double pitch) {
    pitch_ = std::fmod(pitch, glm::pi<double>() * 2.0);
    update_view_matrix();
}

void Camera::set_position(const glm::dvec3& position) {
    position_ = position;
    update_view_matrix();
}

void Camera::update_view_matrix() {
    double cos_yaw = std::cos(yaw_);
    double sin_yaw = std::sin(yaw_);
    double cos_pitch = std::cos(pitch_);
    double sin_pitch = std::sin(pitch_);

    double cos_up = std::cos(pitch_ + glm::pi<double>() / 2.0);
    double sin_up = std::sin(pitch_ + glm::pi<double>() / 2.0);

    glm::dvec3 target{cos_yaw * sin_pitch, sin_yaw * sin_pitch, cos_pitch};
    glm::dvec3 camera_up{cos_yaw * sin_up, sin_yaw * sin_up, cos_up};
    auto pos = position();

    view_matrix_ = glm::lookAt(pos, target + pos, camera_up);
}