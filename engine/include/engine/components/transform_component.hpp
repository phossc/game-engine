#ifndef ENGINE_COMPONENTS_TRANSFORM_COMPONENT_HPP
#define ENGINE_COMPONENTS_TRANSFORM_COMPONENT_HPP

#include <LinearMath/btMotionState.h>
#include <glm/ext/quaternion_float.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include <engine/core/component.hpp>

namespace engine {

class Transform_component : public core::Component {
public:
    COMPONENT("505e8991-d2de-4c07-9371-62d6ed8619fa");
    DEPENDENCIES();

    glm::vec3 position() const noexcept { return position_; }
    void set_position(const glm::vec3& pos) noexcept {
        position_ = pos;
        transform_needs_update_ = true;
    }

    glm::vec3 scale() const noexcept { return scale_; }
    void set_scale(const glm::vec3& scale) noexcept {
        scale_ = scale;
        transform_needs_update_ = true;
    }

    glm::quat orientation() const noexcept { return orientation_; }
    void set_orientation(const glm::quat& orientation) noexcept {
        orientation_ = orientation;
        transform_needs_update_ = true;
    }

    void rotate(const glm::quat& rotation) noexcept {
        orientation_ = rotation * orientation_;
        transform_needs_update_ = true;
    }

    const glm::mat4& transform() const noexcept;

    // Bullet motion state integration
    void getWorldTransform(btTransform& worldTrans) const;
    void setWorldTransform(const btTransform& worldTrans);

private:
    glm::vec3 position_{0, 0, 0};
    glm::vec3 scale_{1, 1, 1};
    glm::quat orientation_{0, 0, 0, 1};
    mutable glm::mat4 transform_;
    mutable bool transform_needs_update_ = true;
};

} // namespace engine

#endif /* ENGINE_COMPONENTS_TRANSFORM_COMPONENT_HPP */