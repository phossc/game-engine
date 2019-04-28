#ifndef ENGINE_UPDATE_PRIORITIES_HPP
#define ENGINE_UPDATE_PRIORITIES_HPP

#include <cstdint>

namespace engine {

enum class Update_priority : std::int32_t {
    input = 100,
    physics = 2000,
    rendering = 10000
};

} // namespace engine

#endif /* ENGINE_UPDATE_PRIORITIES_HPP */