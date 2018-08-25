#ifndef GAME_GAME_HPP
#define GAME_GAME_HPP

namespace game {

class Game final {
public:
    void register_components();
    void attach_system_components();
};

} // namespace game

#endif /* GAME_GAME_HPP */
