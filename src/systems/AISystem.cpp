///
/// AISystem.cpp
/// FlecsPong
///
/// Refer to LICENSE.txt for more details.
///

#include <flecs.h>


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

flecs::entity AISystem::findBall()
{
    flecs::entity target_ball = m_game->m_ecs.entity().null();
//    flecs::entity *target_ball;
//    target_ball = m_game->m_ecs.entity().null();

    flecs::query<Ball, const Position, const Velocity> q =
            m_game->m_ecs.query<Ball, const Position, const Velocity>();

    double max_x = 0.0;
    q.iter([&](flecs::iter& it, Ball *tball, const Position *tball_pos, const Velocity *tball_vel) {
//    m_game->m_ecs.each([&](flecs::entity bb, Ball& tball, Position& tball_pos, Velocity& tball_vel) {
    for (auto i : it) {
        if(tball_vel[i].m_vel_x>=0 and tball_pos[i].m_x>max_x) {
            target_ball = it.entity(i);
            max_x = tball_pos[i].m_x;
        }
    }
    });
    return target_ball;
}

void AISystem::aiSystem_process(flecs::iter &it, const AI *ai, const Paddle *pad, Velocity *vel,
                      const Position *pos, const Sprite *spr)
{
    AISystem * aisystem = static_cast<AISystem*>(it.ctx());

//    flecs::entity target_ball = aisystem->m_game->m_ecs.entity().null();
//    flecs::entity *target_ball;
//    target_ball = 0;
    flecs::entity target_ball;
    flecs::filter<const Ball, const Position, const Velocity> q =
            it.world().filter<const Ball, const Position, const Velocity>()
           ;
    double max_x = 0.0;
//    q.iter([&](flecs::iter& itb,const  Ball *tball, const Position *tball_pos1, const Velocity *tball_vel1) {
//    for (auto ib : itb) {
//        if(tball_vel1[ib].m_vel_x>=0 and tball_pos1[ib].m_x>max_x) {
//            target_ball = itb.entity(ib);
//            max_x = tball_pos1[ib].m_x;
//        }
//    }
//    });
    q.each([&](flecs::entity bb, const Ball& tball, const Position& tball_pos1, const Velocity& tball_vel1) {
        if(tball_vel1.m_vel_x>=0 and tball_pos1.m_x>max_x) {
            target_ball = bb;
            max_x = tball_pos1.m_x;
        }
    });

//    auto target_ball = aisystem->findBall();
//    auto target_ball = flecs::entity::null();

    for (auto i : it) {
    if(!target_ball.is_valid()) {
        if (240.0 >= (pos[i].m_y+(3*spr[i].m_height/4)))
        {
            vel[i].m_vel_y = (pad[i].m_velocity);
        }
        else if (240.0 <= (pos[i].m_y+(spr[i].m_height/4)))
        {
            vel[i].m_vel_y = (-pad[i].m_velocity);
        }
        else
        {
            vel[i].m_vel_y = 0.0;
        }
    }
    else
    {
        const auto& ball_pos = target_ball.get<Position>()[0];
        if (ball_pos.m_y > (pos[i].m_y+(3*spr[i].m_height/4)))
        {
            vel[i].m_vel_y = pad[i].m_velocity;
        }
        else if (ball_pos.m_y < (pos[i].m_y+(spr[i].m_height/4)))
        {
            vel[i].m_vel_y = (-pad[i].m_velocity);
        }
        else
        {
            vel[i].m_vel_y = 0.0;
        }
    }
    }
}


void aiSystem_process(flecs::iter &it, const AI *ai, const Paddle *pad, Velocity *vel,
                      const Position *pos, const Sprite *spr)
{
    AISystem * aisystem = static_cast<AISystem*>(it.ctx());

    auto target_ball = aisystem->findBall();
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
