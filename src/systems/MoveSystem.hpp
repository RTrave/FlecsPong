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
#include "../components/All.hpp"

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
    Paddle::MoveDirection m_player_movement;
};

void moveSystem_processPlayer(flecs::iter &it, Player *plr, Position *pos);
void moveSystem_processBall(flecs::iter &it, Ball *ball, Position *pos);
void moveSystem_process(flecs::iter &it, Velocity *vel, Position *pos, Sprite *spr);

} // namespace ep

#endif
