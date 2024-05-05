///
/// AISystem.hpp
/// FlecsPong
///
/// Refer to LICENSE.txt for more details.
///

#ifndef FLECSPONG_SYSTEMS_AISYSTEM_HPP_
#define FLECSPONG_SYSTEMS_AISYSTEM_HPP_

//#include "../core/Window.hpp"

namespace fp
{

class Game;
class Window;

///
/// This system will ensure the ai is always following the ball, by centering the ai paddle
/// onto the ball.
///
class AISystem final
{
public:
    ///
    /// Default constructor.
    ///
    AISystem(Game *game, Window *window);

    ///
    /// Default destructor.
    ///
    ~AISystem() = default;

    ///
    /// Find ball to target
    ///
    flecs::entity findBall();

    ///
    /// Pointer to main game data.
    ///
    Game *m_game;

    ///
    /// Pointer to window data.
    ///
    Window *m_window;
};

void aiSystem_process(flecs::iter &it, AI *ai, Paddle *pad,
                      Velocity *vel, Position *pos, Sprite *spr);

} // namespace ep

#endif
