///
/// AISystem.cpp
/// FlecsPong
///
/// Refer to LICENSE.txt for more details.
///

#include <flecs/flecs.h>


#include "../components/All.hpp"
#include "../core/Window.hpp"
#include "../core/Game.hpp"

#include "AISystem.hpp"

namespace fp
{

AISystem::AISystem(Game *game, Window* window)
{
    m_game = game;
    m_window = window;
}

flecs::entity AISystem::findBall(flecs::world world)
{
    flecs::entity target_ball = m_game->m_ecs.entity().null();
    double max_x = 0.0;
    flecs::filter<const Ball, const Position, const Velocity> q =
            world.filter<const Ball, const Position, const Velocity>()
           ;
    q.each([&](flecs::entity bb, const Ball& tball, const Position& tball_pos1, const Velocity& tball_vel1) {
        if(tball_vel1.m_vel_x>=0 and tball_pos1.m_x>max_x) {
            target_ball = bb;
            max_x = tball_pos1.m_x;
        }
    });

//    world.each([&](flecs::entity bb, Ball& tball, Position& tball_pos, Velocity& tball_vel) {
//        if(tball_vel.m_vel_x>=0 and tball_pos.m_x>max_x) {
//            target_ball = bb;
//            max_x = tball_pos.m_x;
//        }
//    });
    return target_ball;
}

void aiSystem_process(flecs::iter &it, const AI *ai, const Paddle *pad, Velocity *vel,
                      const Position *pos, const Sprite *spr)
{
    AISystem * aisystem = static_cast<AISystem*>(it.ctx());

    auto target_ball = aisystem->findBall(it.world());
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
