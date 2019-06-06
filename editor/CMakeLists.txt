add_executable(editor)
target_include_directories(editor PRIVATE "./")

target_sources(editor
    PRIVATE
        "editor/editor.hpp"
        "editor/main.cpp"
    )

target_link_libraries(editor PRIVATE engine game)
