#ifndef GAME_GAME_HPP
#define GAME_GAME_HPP

#include <engine/core/component.hpp>

namespace game {

class Game final : public engine::core::Component {
public:
    COMPONENT("b1aec0b5-06ad-4ee5-8578-3e0b30cbb1ca");
    DEPENDENCIES();

    void register_components();
    void attach_system_components();

    //! Functions as the game's entry point.
    void activate() override;
    void deactivate() override;
};

} // namespace game

#endif /* GAME_GAME_HPP */
