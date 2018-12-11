#ifndef GAME_GAME_SETUP_HPP
#define GAME_GAME_SETUP_HPP

namespace game {

class Game_setup final {
public:
    void register_components();
    void attach_system_components();
};

} // namespace game

#endif /* GAME_GAME_SETUP_HPP */