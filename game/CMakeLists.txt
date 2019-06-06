add_library(game STATIC)
target_include_directories(game PUBLIC "./")

target_sources(game
    PRIVATE
        "game/game.cpp"
        "game/game.hpp"
    )

target_link_libraries(game PRIVATE engine)
