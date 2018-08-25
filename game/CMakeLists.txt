add_library(game STATIC)
target_include_directories(game PUBLIC "include")

target_sources(game
    PRIVATE
        "include/game/game.hpp"
        "src/game.cpp"
    )

target_link_libraries(game PRIVATE engine)
