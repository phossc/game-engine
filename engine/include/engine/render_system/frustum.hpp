#ifndef RENDER_SYSTEM_FRUSTUM_HPP
#define RENDER_SYSTEM_FRUSTUM_HPP

#include <glm/mat4x4.hpp>

class Frustum {
public:
    Frustum() { update_projection_matrix(); }

    void set_frustum_scale(float scale) noexcept;
    void set_fov(float fov) noexcept;
    void set_far_clip_plane(float far) noexcept;
    void set_near_clip_plane(float near) noexcept;
    void set_aspect_ratio(float aspect_ratio) noexcept;

    const glm::dmat4& projection_matrix() const noexcept {
        return projection_matrix_;
    };

    float frustum_scale() const noexcept { return frustum_scale_; }
    float aspect_ratio() const noexcept { return aspect_ratio_; }
    float fov() const noexcept { return fov_; }
    float far_clip_plane() const noexcept { return far_clip_plane_; }
    float near_clip_plane() const noexcept { return near_clip_plane_; }

private:
    float frustum_scale_ = 1;
    float aspect_ratio_ = 16.0f / 9.0f;
    float fov_ = 90.0f;
    float far_clip_plane_ = 45.0f;
    float near_clip_plane_ = 1.0f;
    glm::dmat4 projection_matrix_;
    void update_projection_matrix() noexcept;
};

#endif /* RENDER_SYSTEM_FRUSTUM_HPP */