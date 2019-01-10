#include <engine/components/transform_component.hpp>

#include <type_traits>

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat3x3.hpp>

namespace engine {

const glm::mat4& Transform_component::transform() const noexcept {
    if (!transform_needs_update_) {
        return transform_;
    }

    transform_needs_update_ = false;
    transform_ = glm::translate(glm::mat4(1.0f), position_) *
                 glm::mat4_cast(orientation_) *
                 glm::scale(glm::mat4(1.0f), scale_);

    return transform_;
}

void Transform_component::getWorldTransform(btTransform& worldTrans) const {
    static_assert(std::is_same_v<btScalar, decltype(transform_)::value_type>,
                  "Bullet and glm use different underlying scalar types");

    const auto& t = transform();
    glm::mat3 pure_rotation;
    pure_rotation[0] = glm::normalize(t[0]);
    pure_rotation[1] = glm::normalize(t[1]);
    pure_rotation[2] = glm::normalize(t[2]);

    worldTrans.getBasis().setFromOpenGLSubMatrix(glm::value_ptr(pure_rotation));
    worldTrans.getOrigin().setX(position_.x);
    worldTrans.getOrigin().setY(position_.y);
    worldTrans.getOrigin().setZ(position_.z);
}

void Transform_component::setWorldTransform(const btTransform& worldTrans) {
    static_assert(std::is_same_v<btScalar, decltype(transform_)::value_type>,
                  "Bullet and glm use different underlying scalar types");

    const auto& pos = worldTrans.getOrigin();
    position_.x = pos.getX();
    position_.y = pos.getY();
    position_.z = pos.getZ();

    auto quat = worldTrans.getRotation();
    orientation_.x = quat.getX();
    orientation_.y = quat.getY();
    orientation_.z = quat.getZ();
    orientation_.w = quat.getW();

    transform_needs_update_ = true;
}

} // namespace engine