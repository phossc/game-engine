add_library(glad STATIC)
target_include_directories(glad PUBLIC "include")
target_compile_features(glad PRIVATE c_std_11)

target_sources(glad
    PRIVATE
        "include/glad/glad.h"
        "include/KHR/khrplatform.h"
        "src/glad.c"
    )