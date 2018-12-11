add_executable(launcher)
target_include_directories(launcher PRIVATE "include")

target_sources(launcher
    PRIVATE
        "include/launcher/launcher.hpp"
        "include/launcher/launcher_setup.hpp"
        "src/launcher.cpp"
        "src/launcher_setup.cpp"
        "src/main.cpp"
    )

target_link_libraries(launcher PRIVATE engine game)
