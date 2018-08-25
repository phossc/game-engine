add_library(engine STATIC)
target_include_directories(engine PUBLIC "include")
target_compile_features(engine PUBLIC cxx_std_17)
target_compile_options(engine PUBLIC $<$<CXX_COMPILER_ID:AppleClang>:-Wall -Wextra -pedantic -stdlib=libc++>)
target_compile_options(engine PUBLIC $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra -pedantic>)
target_compile_options(engine PUBLIC $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -pedantic>)
target_compile_options(engine PUBLIC $<$<CXX_COMPILER_ID:MSVC>:/W4>)

target_sources(engine
    PRIVATE
        "include/engine/core/system.hpp"
        "src/core/system.cpp"
    )
