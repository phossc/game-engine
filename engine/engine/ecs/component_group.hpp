#ifndef ENGINE_ECS_COMPONENT_GROUP_HPP
#define ENGINE_ECS_COMPONENT_GROUP_HPP

#include <vector>

namespace engine::ecs {

template <typename ComponentType>
class Component_group {
public:
private:
    std::vector<typename ComponentType::Group_tuple> groups_;
};

} // namespace engine::ecs

#endif /* ENGINE_ECS_COMPONENT_GROUP_HPP */