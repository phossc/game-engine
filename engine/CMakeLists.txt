add_library(engine STATIC)
target_include_directories(engine PUBLIC "include")
target_compile_features(engine PUBLIC cxx_std_17)
target_compile_options(engine PUBLIC $<$<CXX_COMPILER_ID:AppleClang>:-Wall -Wextra -pedantic -stdlib=libc++>)
target_compile_options(engine PUBLIC $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra -pedantic>)
target_compile_options(engine PUBLIC $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -pedantic>)
target_compile_options(engine PUBLIC $<$<CXX_COMPILER_ID:MSVC>:/W4>)

target_sources(engine
    PRIVATE
        "include/engine/array_view.hpp"
        "include/engine/components/transform_component.hpp"
        "include/engine/core/clock.hpp"
        "include/engine/core/component.hpp"
        "include/engine/core/component_entity.hpp"
        "include/engine/core/component_registry.hpp"
        "include/engine/core/ecs.hpp"
        "include/engine/core/entity.hpp"
        "include/engine/core/entity_id.hpp"
        "include/engine/core/game_entity.hpp"
        "include/engine/core/game_loop.hpp"
        "include/engine/core/launch.hpp"
        "include/engine/core/profiler.hpp"
        "include/engine/core/system.hpp"
        "include/engine/core/system_entity.hpp"
        "include/engine/core/update_system.hpp"
        "include/engine/core/uuid.hpp"
        "include/engine/glfw/glfw.hpp"
        "include/engine/input_system/action_map.hpp"
        "include/engine/input_system/input_system.hpp"
        "include/engine/input_system/input_types.hpp"
        "include/engine/physics_system/physics_system.hpp"
        "include/engine/render_system/camera.hpp"
        "include/engine/render_system/camera_manager.hpp"
        "include/engine/render_system/frustum.hpp"
        "include/engine/render_system/mesh.hpp"
        "include/engine/render_system/render_system.hpp"
        "include/engine/render_system/shader.hpp"
        "include/engine/render_system/shader_program.hpp"
        "include/engine/render_system/vertex_formats.hpp"
        "include/engine/update_priorities.hpp"
        "include/engine/window_manager/window.hpp"
        "src/components/transform_component.cpp"
        "src/core/clock.cpp"
        "src/core/component_entity.cpp"
        "src/core/component_registry.cpp"
        "src/core/ecs.cpp"
        "src/core/game_entity.cpp"
        "src/core/game_loop.cpp"
        "src/core/profiler.cpp"
        "src/core/system.cpp"
        "src/core/update_system.cpp"
        "src/core/uuid.cpp"
        "src/glfw/glfw.cpp"
        "src/input_system/action_map.cpp"
        "src/input_system/input_system.cpp"
        "src/physics_system/physics_system.cpp"
        "src/render_system/camera.cpp"
        "src/render_system/camera_manager.cpp"
        "src/render_system/frustum.cpp"
        "src/render_system/render_system.cpp"
        "src/render_system/shader.cpp"
        "src/render_system/shader_program.cpp"
        "src/window_manager/window.cpp"
    )

target_link_libraries(engine PUBLIC spdlog glm glad glfw bullet)