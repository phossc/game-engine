add_library(tests_main STATIC)
target_sources(tests_main PRIVATE "tests_main.cpp")
target_link_libraries(tests_main PUBLIC catch)
target_compile_features(tests_main PUBLIC cxx_std_17)

add_subdirectory(engine)