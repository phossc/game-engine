#include <engine/components/transform_component.hpp>

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace engine {

glm::mat4 Transform_component::transform() noexcept {
    if (!transform_needs_update_) {
        return transform_;
    }

    transform_needs_update_ = false;
    transform_ = glm::translate(glm::mat4(1.0f), position_) *
                 glm::mat4_cast(orientation_) *
                 glm::scale(glm::mat4(1.0f), scale_);

    return transform_;
}

} // namespace engine