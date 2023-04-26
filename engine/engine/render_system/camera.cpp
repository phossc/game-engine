#include "engine/render_system/camera.hpp"

#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"

#include <cmath>

namespace engine {

const glm::dmat4& Camera::orientation() const {
    if (orientation_needs_update_) {
        orientation_ = glm::mat4_cast(glm::dquat(glm::dvec3{pitch_, yaw_, 0.0}));

        orientation_needs_update_ = false;
    }

    return orientation_;
}

const glm::dmat4& Camera::view_matrix() const {
    if (view_matrix_needs_update_) {
        view_matrix_ = glm::transpose(orientation());
        view_matrix_ *= glm::translate(glm::dmat4(1.0f), -position_);
        view_matrix_needs_update_ = false;
    }

    return view_matrix_;
}

} // namespace engine