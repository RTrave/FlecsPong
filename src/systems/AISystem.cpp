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

//flecs::entity& findBall(flecs::world ecs)
//{
//    ecs.each([&](flecs::entity tball, Position& tball_pos, Velocity& tball_vel) {
//        if(tball_vel.m_vel_x>=0) {
//            return tball;
//        }
//    });
//    return ecs.;
//}

void aiSystem_process(flecs::iter &it, AI *ai, Paddle *pad, Velocity *vel,
                      Position *pos, Sprite *spr)
{
//    AISystem * aisystem = static_cast<AISystem*>(it.ctx());
    flecs::entity target_ball = it.world().entity().null();
    double max_x = 0.0;
    it.world().each([&](flecs::entity bb, Ball& tball, Position& tball_pos, Velocity& tball_vel) {
        if(tball_vel.m_vel_x>=0 and tball_pos.m_x>max_x) {
            target_ball = bb;
            max_x = tball_pos.m_x;
        }
    });

    if(!target_ball.is_valid()) {
        if (240.0 >= (pos->m_y+(3*spr->m_height/4)))
        {
            vel->m_vel_y = (pad->m_velocity);
        }
        else if (240.0 <= (pos->m_y+(spr->m_height/4)))
        {
            vel->m_vel_y = (-pad->m_velocity);
        }
        else
        {
            vel->m_vel_y = 0.0;
        }
    }
    else
    {
        const auto& ball_pos = target_ball.get<Position>()[0];
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
}

} // namespace ep
