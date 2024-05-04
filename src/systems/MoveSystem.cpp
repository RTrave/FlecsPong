///
/// MoveSystem.cpp
/// FlecsPong
///
/// Refer to LICENSE.txt for more details.
///

#include <flecs.h>

#include "../components/Ball.hpp"
#include "../components/Position.hpp"
#include "../components/Sprite.hpp"
#include "../Random.hpp"

#include "MoveSystem.hpp"

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
	void MoveSystem::on_key_down(const KeyDown& key_down) noexcept
	{
		switch (key_down.m_keycode)
		{
			case SDLK_w:
				m_player_movement = Player::MoveDirection::NORTH;
				break;

			case SDLK_s:
				m_player_movement = Player::MoveDirection::SOUTH;
				break;
		}
	}

	void MoveSystem::on_key_up(const KeyUp& key_up) noexcept
	{
		if (key_up.m_keycode == SDLK_w || key_up.m_keycode == SDLK_s)
		{
			m_player_movement = Player::MoveDirection::STOPPED;
		}
	}

	void MoveSystem::update(const double time, flecs::world& world)
	{
		// We only need to update the player position, since the ai wil be managed by the ai system.
		auto& plr = world.lookup("PlayerPaddle").get_mut<Player>()[0];
        auto& player_pos = world.lookup("PlayerPaddle").get_mut<Position>()[0];
        const auto player_spr = world.lookup("PlayerPaddle").get<Sprite>()[0];
        plr.m_movement = m_player_movement;

        if (plr.m_movement == Player::MoveDirection::NORTH)
        {
            player_pos.m_y -= 0.15 * time;
        }
        else if (plr.m_movement == Player::MoveDirection::SOUTH)
        {
            player_pos.m_y += 0.15 * time;
        }

        // Lock to screen.
        if (player_pos.m_y < 0.0)
        {
            player_pos.m_y = 0.0;
        }
        else if (player_pos.m_y > (480.0 - player_spr.m_height)) // screen width - sprite width
        {
            player_pos.m_y = (480.0 - player_spr.m_height);
        }

		// Next, we want to update the balls position and move it according to the directions its currently travelling.
        auto& ball = world.lookup("Ball").get_mut<Ball>()[0];
        auto& ball_pos = world.lookup("Ball").get_mut<Position>()[0];
        const auto& ball_spr = world.lookup("Ball").get<Sprite>()[0];

        // Allow the ball to move based on a fixed-timestep loop.
        ball_pos.m_x += ball.m_vel_x * time;
        ball_pos.m_y += ball.m_vel_y * time;

        // Ensure ball can be reset.
        if (ball_pos.m_x < 0.0)
        {
            // Ball passed the player paddle, reset it.
            ball_pos.m_x = (640.0 / 2.0) - ball_spr.m_radius*2;
            ball_pos.m_y = (480.0 / 2.0) - ball_spr.m_radius*2;

            ball.m_vel_x = randomize_velocity_dir(ball.initial_x_vel());
            ball.m_vel_y = randomize_velocity_dir(ball.initial_y_vel());
        }
        else if (ball_pos.m_x > (640.0 - ball_spr.m_radius*2)) // screen width - sprite width
        {
            // Ball passed the ai paddle, reset it.
            ball_pos.m_x = (640.0 / 2.0) - ball_spr.m_radius*2;
            ball_pos.m_y = (480.0 / 2.0) - ball_spr.m_radius*2;

            ball.m_vel_x = randomize_velocity_dir(ball.initial_x_vel());
            ball.m_vel_y = randomize_velocity_dir(ball.initial_y_vel());
        }

        // Lock to screen.
        if (ball_pos.m_y < 0.0)
        {
            // Reverse ball, "bouncing" it.
            ball_pos.m_y = 0.0;
            ball.m_vel_y *= -1;
        }
        else if (ball_pos.m_y > (480.0 - ball_spr.m_radius*2)) // screen height - sprite height
        {
            // Reverse ball, "bouncing" it.
            ball_pos.m_y = (480.0 - ball_spr.m_radius*2);
            ball.m_vel_y *= -1;
        }
	}
} // namespace ep
