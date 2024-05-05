///
/// MoveSystem.cpp
/// FlecsPong
///
/// Refer to LICENSE.txt for more details.
///

#include <flecs.h>

#include "../components/Ball.hpp"
#include "../components/Position.hpp"
#include "../Random.hpp"

#include "MoveSystem.hpp"

namespace fp
{

void moveSystem_process(flecs::iter &it, Velocity *vel, Position *pos, Sprite *spr)
{
    const double time = it.delta_time();

    // Iterate entities
    for (auto i : it) {

        pos[i].m_x += vel[i].m_vel_x * time;
        pos[i].m_y += vel[i].m_vel_y * time;

        // Lock to screen.
        if (pos[i].m_y < 0.0)
        {
            pos[i].m_y = 0.0;
            if(it.entity(i).has<Ball>())
                vel[i].m_vel_y = -vel[i].m_vel_y;
            else
                vel[i].m_vel_y = 0.0;
        }
        else if (pos[i].m_y > (480.0 - spr[i].m_height)) // screen width - sprite width
        {
            pos[i].m_y = (480.0 - spr[i].m_height);
            if(it.entity(i).has<Ball>())
                vel[i].m_vel_y = -vel[i].m_vel_y;
            else
                vel[i].m_vel_y = 0.0;
        }
    }
}

} // namespace ep
