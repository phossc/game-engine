#include "engine/components/transform_c.hpp"

#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/mat3x3.hpp"

#include <type_traits>

namespace engine {

const glm::mat4& Transform_c::transform() const noexcept {
    if (!transform_needs_update_) {
        return transform_;
    }

    transform_needs_update_ = false;
    transform_ = glm::translate(glm::mat4(1.0f), position_) * glm::mat4_cast(orientation_) *
                 glm::scale(glm::mat4(1.0f), scale_);

    return transform_;
}

void Transform_c::getWorldTransform(btTransform& worldTrans) const {
    static_assert(std::is_same_v<btScalar, decltype(transform_)::value_type>,
                  "Bullet and glm use different underlying scalar types");

    if (scale_ == glm::vec3{1, 1, 1}) {
        worldTrans.setFromOpenGLMatrix(glm::value_ptr(transform()));
    } else {
        auto rigid_transform =
                glm::translate(glm::mat4(1.0f), position_) * glm::mat4_cast(orientation_);

        worldTrans.setFromOpenGLMatrix(glm::value_ptr(rigid_transform));
    }
}

void Transform_c::setWorldTransform(const btTransform& worldTrans) {
    static_assert(std::is_same_v<btScalar, decltype(transform_)::value_type>,
                  "Bullet and glm use different underlying scalar types");

    const auto& pos = worldTrans.getOrigin();
    position_.x = pos.getX();
    position_.y = pos.getY();
    position_.z = pos.getZ();

    auto quat = worldTrans.getRotation();
    orientation_.x = quat.getW();
    orientation_.y = quat.getX();
    orientation_.z = quat.getY();
    orientation_.w = quat.getZ();

    transform_needs_update_ = true;
}

} // namespace engine