add_executable(editor)
target_include_directories(editor PRIVATE "include")

target_sources(editor
    PRIVATE
        "include/editor/editor.hpp"
        "src/editor.cpp"
        "src/main.cpp"
    )

target_link_libraries(editor PRIVATE engine game)
