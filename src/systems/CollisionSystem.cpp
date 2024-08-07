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

namespace fp {

void collisionSystem_process(flecs::iter &it)
{
    CollisionHolder *holder = static_cast<CollisionHolder*>(it.ctx());
    const auto &player1_pos = holder->player1.get<Position>()[0];
    const auto &player1_spr = holder->player1.get<Sprite>()[0];
    const auto &player2_pos = holder->player2.get<Position>()[0];
    const auto &player2_spr = holder->player2.get<Sprite>()[0];

    // Player1 bounding box.
    const SDL_Rect player1_bb { static_cast<int>(player1_pos.m_x),
            static_cast<int>(player1_pos.m_y), player1_spr.m_width,
            player1_spr.m_height };
    // Player2 bounding box
    const SDL_Rect player2_bb { static_cast<int>(player2_pos.m_x),
            static_cast<int>(player2_pos.m_y), player2_spr.m_width,
            player2_spr.m_height };

//    int maxballs_reload = 5000;

    while (it.next())
    {
        auto ball = it.field<Ball>(0);
        auto pos = it.field<Position>(1);
        auto vel = it.field<Velocity>(2);
        auto spr = it.field<const Sprite>(3);

        for (auto i : it)
        {
            // Lock Ball to screen.
            if (pos[i].m_y < 0.0)
            {
                pos[i].m_y = 0.0;
                vel[i].m_vel_y = -vel[i].m_vel_y;
            }
            else if (pos[i].m_y > (480.0 - spr[i].m_height)) // screen width - sprite width
            {
                pos[i].m_y = (480.0 - spr[i].m_height);
                vel[i].m_vel_y = -vel[i].m_vel_y;
            }

            // If the ball is currently immune we can count down the ticks and return.
            if (ball[i].m_bounce_immune_ticks > 0)
            {
                ball[i].m_bounce_immune_ticks--;
            }
            else
            {
                // Ball bounding box.
                const SDL_Rect ball_bb { static_cast<int>(pos[i].m_x),
                        static_cast<int>(pos[i].m_y), spr[i].m_width,
                        spr[i].m_height };

                // Calculate collisions and act on them.
                if (SDL_HasIntersection(&player1_bb, &ball_bb) == SDL_TRUE)
                {
                    // Reverse ball, "bouncing" it.
                    vel[i].m_vel_x *= -1;
                    // Set bounce immunity for a few ticks to prevent ball from getting stuck inside the paddle.
                    ball[i].m_bounce_immune_ticks = 30;
                }

                if (SDL_HasIntersection(&player2_bb, &ball_bb) == SDL_TRUE)
                {
                    // Reverse ball, "bouncing" it.
                    vel[i].m_vel_x *= -1;
                    // Set bounce immunity for a few ticks to prevent ball from getting stuck inside the paddle.
                    ball[i].m_bounce_immune_ticks = 60;
                }
            }

            // Ball pass paddle: point
            if (pos[i].m_x < 0.0)
            {
                holder->score_player2++;
                // Ball passed the player paddle, reset it.
                pos[i].m_x = (640.0 / 2.0) - 8.0;
                pos[i].m_y = (480.0 / 2.0) - 8.0;

                vel[i].m_vel_x = randomize_velocity_dir(
                        ball[i].m_initial_vel_x);
                vel[i].m_vel_y = randomize_velocity_dir(
                        ball[i].m_initial_vel_y);
            }
            else if (pos[i].m_x > (640.0 - 16.0)) // screen width - sprite width
            {
                holder->score_player1++;
                // Ball passed the ai paddle, reset it.
                pos[i].m_x = (640.0 / 2.0) - 8.0;
                pos[i].m_y = (480.0 / 2.0) - 8.0;

                vel[i].m_vel_x = randomize_velocity_dir(
                        ball[i].m_initial_vel_x);
                vel[i].m_vel_y = randomize_velocity_dir(
                        ball[i].m_initial_vel_y);
            }
        }
    }
}

} // namespace ep

