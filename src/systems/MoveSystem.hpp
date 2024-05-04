///
/// MoveSystem.hpp
/// FlecsPong
///
/// Refer to LICENSE.txt for more details.
///

#ifndef FLECSPONG_SYSTEMS_MOVESYSTEM_HPP_
#define FLECSPONG_SYSTEMS_MOVESYSTEM_HPP_

#include <flecs.h>
#include <SDL2/SDL_events.h>

#include "../core/Window.hpp"
#include "../components/Player.hpp"

namespace fp
{
///
/// This class will take data from the appropriate components
/// and update the position based on input.
///
class MoveSystem final
{
public:
    ///
    /// Default constructor.
    ///
    MoveSystem() = default;

    ///
    /// Default destructor.
    ///
    ~MoveSystem() = default;

    ///
    /// Current movement of player.
    ///
    Player::MoveDirection m_player_movement;
};

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
    InputSystem(Window* window);

    ///
    /// Default destructor.
    ///
    ~InputSystem() = default;

    ///
    /// Pointer to window data.
    ///
    Window* m_window;
};

void inputSystem_process(flecs::iter &it, Player *player, Position *pos);
void moveSystem_processPlayer(flecs::iter &it, Player *plr, Position *pos);
void moveSystem_processBall(flecs::iter &it, Ball *ball, Position *pos);

} // namespace ep

#endif
