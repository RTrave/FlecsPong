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

void moveSystem_process(flecs::iter &it, size_t i,
    Velocity &vel, Position &pos, const Sprite &spr)
{
    const double time = it.delta_time()*1000;

    // Update position by adding velocity
    pos.m_x += vel.m_vel_x * time;
    pos.m_y += vel.m_vel_y * time;

    // Lock Paddle to screen.
    if (pos.m_y < 0.0)
    {
        if(it.entity(i).has<Paddle>())
        {
            pos.m_y = 0.0;
        }
    }
    else if (pos.m_y > (480.0 - spr.m_height)) // screen width - sprite width
    {
        if(it.entity(i).has<Paddle>())
        {
            pos.m_y = (480.0 - spr.m_height);
        }
    }
}

} // namespace ep
