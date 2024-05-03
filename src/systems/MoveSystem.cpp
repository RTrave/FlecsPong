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
	void MoveSystem::on_key_down(const KeyDown& key_down, flecs::world& world) noexcept
	{
//        auto player_query = world.query<Player>();
	    const auto& player = world.lookup("PlayerPaddle");
        auto& plr = player.get_mut<Player>()[0];
		switch (key_down.m_keycode)
		{
			case SDLK_w:
				m_player_movement = Player::MoveDirection::NORTH;
//		        auto player_query = world.query<Player>();
//		        player_query.each([&](Player& plr) {
		            plr.m_movement = Player::MoveDirection::NORTH;
//		        });
				break;

			case SDLK_s:
				m_player_movement = Player::MoveDirection::SOUTH;
//                auto player_query = world.query<Player>();
//                player_query.each([&](Player& plr) {
                    plr.m_movement = Player::MoveDirection::SOUTH;
//                });
				break;
		}
	}

	void MoveSystem::on_key_up(const KeyUp& key_up, flecs::world& world) noexcept
	{
		if (key_up.m_keycode == SDLK_w || key_up.m_keycode == SDLK_s)
		{
//	        auto plr = world.lookup("PlayerPaddle").get_mut<Player>()[0];
	        const auto& player = world.lookup("PlayerPaddle");
	        auto& plr = player.get_mut<Player>()[0];
	        m_player_movement = Player::MoveDirection::STOPPED;
	        auto player_query = world.query<Player>();
//            player_query.each([&](Player& plr) {
                plr.m_movement = Player::MoveDirection::STOPPED;
//            });
		}
	}



	void MoveSystem::update(const double time, flecs::world& world)
	{
		// We only need to update the player position, since the ai wil be managed by the ai system.
		auto player_query = world.query<Player, Position>();
		player_query.each([&](Player& plr, Position& pos) {
			plr.m_movement = m_player_movement;

			if (plr.m_movement == Player::MoveDirection::NORTH)
			{
				pos.m_y -= 0.15 * time;
			}
			else if (plr.m_movement == Player::MoveDirection::SOUTH)
			{
				pos.m_y += 0.15 * time;
			}

			// Lock to screen.
			if (pos.m_y < 0.0)
			{
				pos.m_y = 0.0;
			}
			else if (pos.m_y > (480.0 - 96.0)) // screen width - sprite width
			{
				pos.m_y = (480.0 - 96.0);
			}
		});

		// Next, we want to update the balls position and move it according to the directions its currently travelling.
		auto ball_query = world.query<Ball, Position>();
		ball_query.each([&](Ball& ball, Position& pos) {
			// Allow the ball to move based on a fixed-timestep loop.
			pos.m_x += ball.m_vel_x * time;
			pos.m_y += ball.m_vel_y * time;

			// Ensure ball can be reset.
			if (pos.m_x < 0.0)
			{
				// Ball passed the player paddle, reset it.
				pos.m_x = (640.0 / 2.0) - 16.0;
				pos.m_y = (480.0 / 2.0) - 16.0;

				ball.m_vel_x = randomize_velocity_dir(ball.initial_x_vel());
				ball.m_vel_y = randomize_velocity_dir(ball.initial_y_vel());
			}
			else if (pos.m_x > (640.0 - 16.0)) // screen width - sprite width
			{
				// Ball passed the ai paddle, reset it.
				pos.m_x = (640.0 / 2.0) - 16.0;
				pos.m_y = (480.0 / 2.0) - 16.0;

				ball.m_vel_x = randomize_velocity_dir(ball.initial_x_vel());
				ball.m_vel_y = randomize_velocity_dir(ball.initial_y_vel());
			}

			// Lock to screen.
			if (pos.m_y < 0.0)
			{
				// Reverse ball, "bouncing" it.
				pos.m_y = 0.0;
				ball.m_vel_y *= -1;
			}
			else if (pos.m_y > (480.0 - 16.0)) // screen height - sprite height
			{
				// Reverse ball, "bouncing" it.
				pos.m_y = (480.0 - 16.0);
				ball.m_vel_y *= -1;
			}
		});
	}

static flecs::world* o_world;
void MoveSystem::init(flecs::world *world)
{
    m_world = world;
    o_world = world;
    return;
//    m_system = world.system<Player, Position>()
//            .iter([&](flecs::iter& it, Player& p, Position& p_pos) {
    m_system = world->system("Move")
           .iter([&](flecs::iter& it) {

        const double time = it.delta_time();
        // We only need to update the player position, since the ai wil be managed by the ai system.
        auto player_query = world->query<Player, Position>();
        player_query.each([&](Player& plr, Position& pos) {
            plr.m_movement = m_player_movement;
            if (plr.m_movement == Player::MoveDirection::NORTH)
            {
                pos.m_y -= 0.15 * time;
            }
            else if (plr.m_movement == Player::MoveDirection::SOUTH)
            {
                pos.m_y += 0.15 * time;
            }

            // Lock to screen.
            if (pos.m_y < 0.0)
            {
                pos.m_y = 0.0;
            }
            else if (pos.m_y > (480.0 - 96.0)) // screen width - sprite width
            {
                pos.m_y = (480.0 - 96.0);
            }
        });

        // Next, we want to update the balls position and move it according to the directions its currently travelling.
        auto ball_query = world->query<Ball, Position>();
        ball_query.each([&](Ball& ball, Position& pos) {
            // Allow the ball to move based on a fixed-timestep loop.
            pos.m_x += ball.m_vel_x * time;
            pos.m_y += ball.m_vel_y * time;

            // Ensure ball can be reset.
            if (pos.m_x < 0.0)
            {
                // Ball passed the player paddle, reset it.
                pos.m_x = (640.0 / 2.0) - 16.0;
                pos.m_y = (480.0 / 2.0) - 16.0;

                ball.m_vel_x = randomize_velocity_dir(ball.initial_x_vel());
                ball.m_vel_y = randomize_velocity_dir(ball.initial_y_vel());
            }
            else if (pos.m_x > (640.0 - 16.0)) // screen width - sprite width
            {
                // Ball passed the ai paddle, reset it.
                pos.m_x = (640.0 / 2.0) - 16.0;
                pos.m_y = (480.0 / 2.0) - 16.0;

                ball.m_vel_x = randomize_velocity_dir(ball.initial_x_vel());
                ball.m_vel_y = randomize_velocity_dir(ball.initial_y_vel());
            }

            // Lock to screen.
            if (pos.m_y < 0.0)
            {
                // Reverse ball, "bouncing" it.
                pos.m_y = 0.0;
                ball.m_vel_y *= -1;
            }
            else if (pos.m_y > (480.0 - 16.0)) // screen height - sprite height
            {
                // Reverse ball, "bouncing" it.
                pos.m_y = (480.0 - 16.0);
                ball.m_vel_y *= -1;
            }
        });

    });

}

void moveSystem_processPlayer(flecs::iter &it, Player* plr, Position* pos)
{
    const double time = it.delta_time();

//    auto player_query = it.world().query<Player, Position>();
//    player_query.each([&](Player& plr, Position& pos) {
//        plr.m_movement = m_player_movement;
//    printf("Test: %f\n", time);
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
//    });

}

InputSystem::InputSystem(Window *window)
{
    m_window = window;
}

//static Window *s_window;
//
//void inputSystem_init(Window *window)
//{
//    s_window = window;
//}

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
//                  m_dispatcher.trigger<KeyDown>(m_window.m_event.key.keysym.sym);
//                m_move_system.on_key_down(s_window.m_event.key.keysym.sym, it.world());
//                const int keycode_down = s_window.m_event.key.keysym.sym;
                switch (input->m_window->m_event.key.keysym.sym)
                {
                    case SDLK_w:
//                        m_player_movement = Player::MoveDirection::NORTH;
        //              auto player_query = world.query<Player>();
        //              player_query.each([&](Player& plr) {
                            player->m_movement = Player::MoveDirection::NORTH;
        //              });
                        break;

                    case SDLK_s:
//                        m_player_movement = Player::MoveDirection::SOUTH;
        //                auto player_query = world.query<Player>();
        //                player_query.each([&](Player& plr) {
                            player->m_movement = Player::MoveDirection::SOUTH;
        //                });
                        break;
                    case SDLK_ESCAPE:
                        input->m_window->close();
                        break;
                }
//                m_window.on_key_down(s_window.m_event.key.keysym.sym);
                break;

            case SDL_KEYUP:
//                  m_dispatcher.trigger<KeyUp>(m_window.m_event.key.keysym.sym);
//                m_move_system.on_key_up(s_window.m_event.key.keysym.sym, m_ecs);
//                const int keycode_up = s_window.m_event.key.keysym.sym;
                if (input->m_window->m_event.key.keysym.sym == SDLK_w || input->m_window->m_event.key.keysym.sym == SDLK_s)
                {
        //          auto plr = world.lookup("PlayerPaddle").get_mut<Player>()[0];
//                    const auto& player = world.lookup("PlayerPaddle");
//                    auto& plr = player.get_mut<Player>()[0];
//                    m_player_movement = Player::MoveDirection::STOPPED;
//                    auto player_query = world.query<Player>();
        //            player_query.each([&](Player& plr) {
                        player->m_movement = Player::MoveDirection::STOPPED;
        //            });
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

void MoveSystem::processPlayer(flecs::iter &it, Player *plr, Position *pos)
{
    const double time = it.delta_time();

//    auto player_query = it.world().query<Player, Position>();
//    player_query.each([&](Player& plr, Position& pos) {
//        plr.m_movement = m_player_movement;
    printf("Test2: %f\n", time);
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
//    });

}

} // namespace ep
