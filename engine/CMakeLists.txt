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
        "include/engine/core/component.hpp"
        "include/engine/core/component_entity.hpp"
        "include/engine/core/component_registry.hpp"
        "include/engine/core/component_uuid.hpp"
        "include/engine/core/ecs.hpp"
        "include/engine/core/entity.hpp"
        "include/engine/core/entity_id.hpp"
        "include/engine/core/system.hpp"
        "include/engine/core/system_entity.hpp"
        "src/core/component_entity.cpp"
        "src/core/component_registry.cpp"
        "src/core/ecs.cpp"
        "src/core/system.cpp"
    )
