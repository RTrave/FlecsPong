///
/// CollisionSystem.cpp
/// FlecsPong
///
/// Refer to LICENSE.txt for more details.
///

#include <flecs.h>
#include <SDL2/SDL_rect.h>

#include "../components/Ball.hpp"
#include "../components/Position.hpp"
#include "../components/Sprite.hpp"

#include "CollisionSystem.hpp"

namespace fp
{

void collisionSystem_process(flecs::iter &it, Ball *ball, Position* ball_pos, Sprite* ball_spr)
{
    CollisionHolder * holder = static_cast<CollisionHolder*>(it.ctx());
    const auto& ai_pos = holder->ai.get<Position>()[0];
    const auto& ai_spr = holder->ai.get<Sprite>()[0];
    const auto& player_pos = holder->player.get<Position>()[0];
    const auto& player_spr = holder->player.get<Sprite>()[0];

    // If the ball is currently immune we can count down the ticks and return.
    if (ball->m_bounce_immune_ticks > 0)
    {
        ball->m_bounce_immune_ticks--;
    }
    else
    {
        // Ball bounding box.
        const SDL_Rect ball_bb {static_cast<int>(ball_pos->m_x - ball_spr->m_radius), static_cast<int>(ball_pos->m_y - ball_spr->m_radius), ball_spr->m_radius * 2, ball_spr->m_radius * 2};

        // Player bounding box.
        const SDL_Rect player_bb {static_cast<int>(player_pos.m_x), static_cast<int>(player_pos.m_y), player_spr.m_width, player_spr.m_height};

        // AI bounding box
        const SDL_Rect ai_bb {static_cast<int>(ai_pos.m_x), static_cast<int>(ai_pos.m_y), ai_spr.m_width, ai_spr.m_height};

        // Calculate collisions and act on them.
        if (SDL_HasIntersection(&player_bb, &ball_bb) == SDL_TRUE)
        {
            // Reverse ball, "bouncing" it.
            ball->m_vel_x *= -1;

            // Set bounce immunity for a few ticks to prevent ball from getting stuck inside the paddle.
            ball->m_bounce_immune_ticks = 5;
        }

        if (SDL_HasIntersection(&ai_bb, &ball_bb) == SDL_TRUE)
        {
            // Reverse ball, "bouncing" it.
            ball->m_vel_x *= -1;

            // Set bounce immunity for a few ticks to prevent ball from getting stuck inside the paddle.
            ball->m_bounce_immune_ticks = 5;
        }
    }
}

} // namespace ep

