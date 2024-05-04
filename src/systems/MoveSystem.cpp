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

void moveSystem_processPlayer(flecs::iter &it, Player* plr, Position* pos)
{
    const double time = it.delta_time();

        if (plr->m_movement == Player::MoveDirection::NORTH)
        {
            pos->m_y -= 0.15 * time;
        }
        else if (plr->m_movement == Player::MoveDirection::SOUTH)
        {
            pos->m_y += 0.15 * time;
        }

        // Lock to screen.
        if (pos->m_y < 0.0)
        {
            pos->m_y = 0.0;
        }
        else if (pos->m_y > (480.0 - 96.0)) // screen width - sprite width
        {
            pos->m_y = (480.0 - 96.0);
        }

}

InputSystem::InputSystem(Window *window)
{
    m_window = window;
}

void inputSystem_process(flecs::iter &it, Player *player, Position *pos)
{
    InputSystem * input = static_cast<InputSystem*>(it.ctx());
    // Process all user and system events.
    while (SDL_PollEvent(&input->m_window->m_event) != 0)
    {
        switch (input->m_window->m_event.type)
        {
            case SDL_QUIT:
                input->m_window->close();
                break;

            case SDL_KEYDOWN:
                switch (input->m_window->m_event.key.keysym.sym)
                {
                    case SDLK_w:
                        player->m_movement = Player::MoveDirection::NORTH;
                        break;

                    case SDLK_s:
                        player->m_movement = Player::MoveDirection::SOUTH;
                        break;
                    case SDLK_ESCAPE:
                        input->m_window->close();
                        break;
                }
                break;

            case SDL_KEYUP:
                if (input->m_window->m_event.key.keysym.sym == SDLK_w || input->m_window->m_event.key.keysym.sym == SDLK_s)
                {
                    player->m_movement = Player::MoveDirection::STOPPED;
                }
                break;
        }
    }
}

void moveSystem_processBall(flecs::iter &it, Ball *ball, Position *pos)
{
    const double time = it.delta_time();
    pos->m_x += ball->m_vel_x * time;
    pos->m_y += ball->m_vel_y * time;

    // Ensure ball can be reset.
    if (pos->m_x < 0.0)
    {
        // Ball passed the player paddle, reset it.
        pos->m_x = (640.0 / 2.0) - 16.0;
        pos->m_y = (480.0 / 2.0) - 16.0;

        ball->m_vel_x = randomize_velocity_dir(ball->initial_x_vel());
        ball->m_vel_y = randomize_velocity_dir(ball->initial_y_vel());
    }
    else if (pos->m_x > (640.0 - 16.0)) // screen width - sprite width
    {
        // Ball passed the ai paddle, reset it.
        pos->m_x = (640.0 / 2.0) - 16.0;
        pos->m_y = (480.0 / 2.0) - 16.0;

        ball->m_vel_x = randomize_velocity_dir(ball->initial_x_vel());
        ball->m_vel_y = randomize_velocity_dir(ball->initial_y_vel());
    }

    // Lock to screen.
    if (pos->m_y < 0.0)
    {
        // Reverse ball, "bouncing" it.
        pos->m_y = 0.0;
        ball->m_vel_y *= -1;
    }
    else if (pos->m_y > (480.0 - 16.0)) // screen height - sprite height
    {
        // Reverse ball, "bouncing" it.
        pos->m_y = (480.0 - 16.0);
        ball->m_vel_y *= -1;
    }
}

} // namespace ep
