cmake_minimum_required(VERSION 3.11.3)
project(GameEngine)

add_subdirectory(engine)
add_subdirectory(game)
add_subdirectory(editor)
add_subdirectory(launcher)

enable_testing()
add_subdirectory(tests)
