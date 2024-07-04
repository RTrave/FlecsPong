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

    ///
    /// InputSystem process
    ///
    bool processPlayerEvent(flecs::iter &iter);
    void processGlobalEvent();

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

    ///
    /// Input Players query
    ///
    flecs::query<const Player, Paddle, Velocity> m_queryPlayer;
};

void inputSystem_process(flecs::iter &iter);
//void inputSystem_process_MT(flecs::iter &iter);

} // namespace ep

#endif
