add_executable(launcher)
target_include_directories(launcher PRIVATE "./")

target_sources(launcher
    PRIVATE
        "launcher/launcher.hpp"
        "launcher/main.cpp"
    )

target_link_libraries(launcher PRIVATE engine game)
