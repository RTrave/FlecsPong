///
/// AISystem.cpp
/// FlecsPong
///
/// Refer to LICENSE.txt for more details.
///

#include "../flecs/flecs.h"


#include "../components/All.hpp"
#include "../core/Window.hpp"
#include "../core/Game.hpp"

#include "AISystem.hpp"

namespace fp
{
AISystem * aisystem;
flecs::world thr_stage;
flecs::query<const Position, const Velocity> qt;

AISystem::AISystem(Game *game, Window* window)
{
    m_game = game;
    m_window = window;
    aisystem = this;
    if(m_game->m_multithreaded)
    {
        thr_stage = m_game->m_ecs.get_stage(1);
        qt = thr_stage.query_builder<const Position, const Velocity>()
            .with<Ball>()
//            .filter()
            .build();
    }
    else
    {
        qt = m_game->m_ecs.query_builder<const Position, const Velocity>()
                    .with<Ball>()
                    .filter()
        //            .instanced()
                    .build();
    }
}

flecs::entity target_e = flecs::entity().null();
double max_x = 0.0;

flecs::entity& AISystem::findBall_MT(flecs::iter &it)
{
    target_e = flecs::entity().null();
    max_x = 0.0;
    qt.iter(it).each([](flecs::entity bb,
                    const Position& bb_pos1, const Velocity& bb_vel1) {

        if(bb_vel1.m_vel_x>=0 and bb_pos1.m_x>max_x) {
            target_e = bb;
            max_x = bb_pos1.m_x;
        }

    });
    return target_e;
}

flecs::entity& AISystem::findBall(flecs::iter &it)
{
    target_e = flecs::entity().null();
    max_x = 0.0;
    qt.iter().each([](flecs::entity bb,
            const Position& bb_pos1, const Velocity& bb_vel1) {

        if(bb_vel1.m_vel_x>=0 and bb_pos1.m_x>max_x) {
            target_e = bb;
            max_x = bb_pos1.m_x;
        }

    });
    return target_e;
}

void aiSystem_process(flecs::iter &it, size_t i,
        const AI &ai, const Paddle &pad,
        Velocity &vel, const Position &pos, const Sprite &spr)
{
//    AISystem * aisystem = static_cast<AISystem*>(it.ctx());

    flecs::entity target_ball;
    if(aisystem->m_game->m_multithreaded)
        target_ball = aisystem->findBall_MT(it);
    else
        target_ball = aisystem->findBall(it);

    if(!target_ball.is_valid()) {
        if (240.0 >= (pos.m_y+(3*spr.m_height/4)))
        {
            vel.m_vel_y = (pad.m_velocity);
        }
        else if (240.0 <= (pos.m_y+(spr.m_height/4)))
        {
            vel.m_vel_y = (-pad.m_velocity);
        }
        else
        {
            vel.m_vel_y = 0.0;
        }
    }
    else
    {
        const auto& ball_pos = target_ball.get<Position>()[0];
        if (ball_pos.m_y > (pos.m_y+(3*spr.m_height/4)))
        {
            vel.m_vel_y = pad.m_velocity;
        }
        else if (ball_pos.m_y < (pos.m_y+(spr.m_height/4)))
        {
            vel.m_vel_y = (-pad.m_velocity);
        }
        else
        {
            vel.m_vel_y = 0.0;
        }
    }
}

} // namespace ep
