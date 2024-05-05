///
/// CollisionSystem.cpp
/// FlecsPong
///
/// Refer to LICENSE.txt for more details.
///

#include <flecs.h>
#include <SDL2/SDL_rect.h>

#include "../components/All.hpp"
#include "../Random.hpp"

#include "CollisionSystem.hpp"

[[nodiscard]] const double randomize_velocity_dir(const double vel)
{
    switch (fp::random(0, 1))
    {
        case 0:
            return vel;
            break;

        case 1:
            return -vel;
            break;
    }
    return 0;
}

namespace fp
{

void collisionSystem_process(flecs::iter &it, Ball *ball, Position *pos,
                             Velocity *vel, Sprite *spr)
{
    CollisionHolder * holder = static_cast<CollisionHolder*>(it.ctx());

    // If the ball is currently immune we can count down the ticks and return.
    if (ball->m_bounce_immune_ticks > 0)
    {
        ball->m_bounce_immune_ticks--;
    }
    else
    {
        const auto& player1_pos = holder->player1.get<Position>()[0];
        const auto& player1_spr = holder->player1.get<Sprite>()[0];
        const auto& player2_pos = holder->player2.get<Position>()[0];
        const auto& player2_spr = holder->player2.get<Sprite>()[0];
        // Ball bounding box.
        const SDL_Rect ball_bb {static_cast<int>(pos->m_x - spr->m_radius), static_cast<int>(pos->m_y - spr->m_radius), spr->m_radius * 2, spr->m_radius * 2};

//        // Paddle bounding box.
//        const SDL_Rect paddle_bb {static_cast<int>(pos->m_x), static_cast<int>(pos->m_y), spr->m_width, spr->m_height};

//
        // Player1 bounding box.
        const SDL_Rect player1_bb {static_cast<int>(player1_pos.m_x), static_cast<int>(player1_pos.m_y), player1_spr.m_width, player1_spr.m_height};

        // Player2 bounding box
        const SDL_Rect player2_bb {static_cast<int>(player2_pos.m_x), static_cast<int>(player2_pos.m_y), player2_spr.m_width, player2_spr.m_height};

        // Calculate collisions and act on them.
        if (SDL_HasIntersection(&player1_bb, &ball_bb) == SDL_TRUE)
        {
            // Reverse ball, "bouncing" it.
            vel->m_vel_x *= -1;

            // Set bounce immunity for a few ticks to prevent ball from getting stuck inside the paddle.
            ball->m_bounce_immune_ticks = 5;
        }

        if (SDL_HasIntersection(&player2_bb, &ball_bb) == SDL_TRUE)
        {
            // Reverse ball, "bouncing" it.
            vel->m_vel_x *= -1;

            // Set bounce immunity for a few ticks to prevent ball from getting stuck inside the paddle.
            ball->m_bounce_immune_ticks = 5;
        }
    }
    // Ensure ball can be reset.
    if (pos->m_x < 0.0)
    {
        // Ball passed the player paddle, reset it.
        pos->m_x = (640.0 / 2.0) - 16.0;
        pos->m_y = (480.0 / 2.0) - 16.0;

        vel->m_vel_x = randomize_velocity_dir(ball->m_initial_vel_x);
        vel->m_vel_y = randomize_velocity_dir(ball->m_initial_vel_y);
    }
    else if (pos->m_x > (640.0 - 16.0)) // screen width - sprite width
    {
        // Ball passed the ai paddle, reset it.
        pos->m_x = (640.0 / 2.0) - 16.0;
        pos->m_y = (480.0 / 2.0) - 16.0;

        vel->m_vel_x = randomize_velocity_dir(ball->m_initial_vel_x);
        vel->m_vel_y = randomize_velocity_dir(ball->m_initial_vel_y);
    }
}

} // namespace ep

