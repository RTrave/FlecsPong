///
/// MoveSystem.hpp
/// FlecsPong
///
/// Refer to LICENSE.txt for more details.
///

#ifndef FLECSPONG_SYSTEMS_INPUTSYSTEM_HPP_
#define FLECSPONG_SYSTEMS_INPUTSYSTEM_HPP_

#include <flecs.h>
#include <SDL2/SDL_events.h>

#include "../components/All.hpp"

namespace fp {

class Game;
class Window;

///
/// This class will take data from the appropriate components
/// and update the position based on input.
///
class InputSystem final
{
public:
    ///
    /// Default constructor.
    ///
    InputSystem(Game *game, Window *window);

    ///
    /// Default destructor.
    ///
    ~InputSystem() = default;

    bool isAzerty() const
    {
        return m_azerty;
    }

    void switchAzerty()
    {
        if (m_azerty)
            m_azerty = false;
        else
            m_azerty = true;
    }

    ///
    /// Pointer to window data.
    ///
    Window *m_window;

    ///
    /// Pointer to main Game object.
    ///
    Game *m_game;

    ///
    /// Switch to/from azerty keyboard
    ///
    bool m_azerty;
};

void inputSystem_process(flecs::iter &it, const Player *player, Paddle *paddle,
        Velocity *velocity);

} // namespace ep

#endif
