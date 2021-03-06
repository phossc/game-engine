add_library(engine STATIC)
target_include_directories(engine PUBLIC "./")
target_compile_features(engine PUBLIC cxx_std_17)
target_compile_options(engine PUBLIC $<$<CXX_COMPILER_ID:AppleClang>:-Wall -Wextra -pedantic -stdlib=libc++>)
target_compile_options(engine PUBLIC $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra -pedantic>)
target_compile_options(engine PUBLIC $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -pedantic>)
target_compile_options(engine PUBLIC $<$<CXX_COMPILER_ID:MSVC>:/W4>)

target_sources(engine
    PRIVATE
        "engine/array_view.hpp"
        "engine/components/transform_c.cpp"
        "engine/components/transform_c.hpp"
        "engine/core/clock.cpp"
        "engine/core/clock.hpp"
        "engine/core/component.hpp"
        "engine/core/component_entity.cpp"
        "engine/core/component_entity.hpp"
        "engine/core/component_registry.cpp"
        "engine/core/component_registry.hpp"
        "engine/core/ecs.cpp"
        "engine/core/ecs.hpp"
        "engine/core/entity.hpp"
        "engine/core/game_entity.cpp"
        "engine/core/game_entity.hpp"
        "engine/core/game_loop.cpp"
        "engine/core/game_loop.hpp"
        "engine/core/profiler.cpp"
        "engine/core/profiler.hpp"
        "engine/core/system.cpp"
        "engine/core/system.hpp"
        "engine/core/system_entity.hpp"
        "engine/core/update_system.cpp"
        "engine/core/update_system.hpp"
        "engine/core/uuid.cpp"
        "engine/core/uuid.hpp"
        "engine/engine.cpp"
        "engine/engine.hpp"
        "engine/glfw/glfw.cpp"
        "engine/glfw/glfw.hpp"
        "engine/id_types.hpp"
        "engine/input_system/action_map.cpp"
        "engine/input_system/action_map.hpp"
        "engine/input_system/input_system.cpp"
        "engine/input_system/input_system.hpp"
        "engine/input_system/input_types.hpp"
        "engine/launch.hpp"
        "engine/physics_system/physics_system.cpp"
        "engine/physics_system/physics_system.hpp"
        "engine/render_system/camera.cpp"
        "engine/render_system/camera.hpp"
        "engine/render_system/camera_manager.cpp"
        "engine/render_system/camera_manager.hpp"
        "engine/render_system/frustum.cpp"
        "engine/render_system/frustum.hpp"
        "engine/render_system/mesh.hpp"
        "engine/render_system/render_system.cpp"
        "engine/render_system/render_system.hpp"
        "engine/render_system/shader.cpp"
        "engine/render_system/shader.hpp"
        "engine/render_system/shader_program.cpp"
        "engine/render_system/shader_program.hpp"
        "engine/render_system/vertex_formats.hpp"
        "engine/update_priorities.hpp"
        "engine/window_manager/window.cpp"
        "engine/window_manager/window.hpp"
    )

target_link_libraries(engine PUBLIC spdlog glm glad glfw bullet strong_typedef)