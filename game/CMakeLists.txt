add_library(game STATIC)
target_include_directories(game PUBLIC "include")

target_sources(game
    PRIVATE
        "include/game/game.hpp"
        "include/game/game_setup.hpp"
        "src/game.cpp"
        "src/game_setup.cpp"
    )

target_link_libraries(game PRIVATE engine)
