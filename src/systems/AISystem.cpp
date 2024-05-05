///
/// AISystem.cpp
/// FlecsPong
///
/// Refer to LICENSE.txt for more details.
///

#include <flecs.h>


#include "../components/All.hpp"

#include "AISystem.hpp"

namespace fp
{

AISystem::AISystem(Window* window)
{
    m_window = window;
}

void aiSystem_process(flecs::iter &it, AI *ai, Paddle *pad, Velocity *vel,
                      Position *pos, Sprite *spr)
{
    const auto& ball_pos = it.world().lookup("Ball").get<Position>()[0];
    if (ball_pos.m_y > (pos->m_y+(3*spr->m_height/4)))
    {
        vel->m_vel_y = pad->m_velocity;
    }
    else if (ball_pos.m_y < (pos->m_y+(spr->m_height/4)))
    {
        vel->m_vel_y = (-pad->m_velocity);
    }
    else
    {
        vel->m_vel_y = 0.0;
    }
}

} // namespace ep
