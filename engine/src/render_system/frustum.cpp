#include "engine/render_system/frustum.hpp"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/trigonometric.hpp"

namespace engine {

void Frustum::set_frustum_scale(float scale) noexcept {
    frustum_scale_ = scale;
    update_projection_matrix();
}

void Frustum::set_fov(float fov) noexcept {
    fov_ = fov;
    update_projection_matrix();
}

void Frustum::set_far_clip_plane(float far) noexcept {
    far_clip_plane_ = far;
    update_projection_matrix();
}
void Frustum::set_near_clip_plane(float near) noexcept {
    near_clip_plane_ = near;
    update_projection_matrix();
}

void Frustum::set_aspect_ratio(float aspect_ratio) noexcept {
    aspect_ratio_ = aspect_ratio;
    update_projection_matrix();
}

void Frustum::update_projection_matrix() noexcept {
    projection_matrix_ = glm::perspective(glm::radians(fov_ / 2), aspect_ratio_,
                                          near_clip_plane_, far_clip_plane_);
    projection_matrix_[0][0] *= frustum_scale_;
    projection_matrix_[1][1] *= frustum_scale_;
}

} // namespace engine