///
/// Game.cpp
/// FlecsPong
///
/// Refer to LICENSE.txt for more details.
///

#include <SDL2/SDL.h>

#include "../components/All.hpp"

#include "../Log.hpp"

#include "Game.hpp"

namespace fp
{
	Game::Game(std::string_view title, const int w, const int h, std::uint32_t flags)
	    : m_window {}
	{
		// Setup SDL2 and stuff.
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0)
		{
			throw_sdl2_exception("Failed to init SDL2.");
		}

		m_window.create(title, w, h, flags);

		// Here, we are creating the entities using EnTT and attaching the relevant components and tags.
		// We can invoke the constructor of the component or tag in the assign() and attach() methods of the registry.
		auto player_paddle = m_ecs.entity("PlayerPaddle")
		        .set([](Sprite &s, Position &pos, Player &p)
            {
                s.m_width = 12;
                s.m_height = 96;
                s.m_colour = SDL_Colour{255, 255, 255, 255};
                s.m_radius = 0;
                pos.m_x = 20.0;
                pos.m_y = 20.0;
                p.m_movement = Player::MoveDirection::STOPPED;
            });
		auto ai_paddle = m_ecs.entity("AIPaddle");
        ai_paddle.set([](Sprite &s)
                    {
                        s.m_width = 12;
                        s.m_height = 96;
                        s.m_colour = SDL_Colour{255, 255, 255, 255};
                        s.m_radius = 0;
                    })
		        .set<Position>({w - 30.0, 20.0})
		        .set<AI>({w - 30.0, 20.0});
		auto ball = m_ecs.entity("Ball")
		        .set([](Sprite &s)
                    {
                        s.m_width = 0;
                        s.m_height = 0;
                        s.m_colour = SDL_Colour{255, 255, 255, 255};
                        s.m_radius = 8;
                    })
//		        .set<Sprite>({8, SDL_Colour {255, 255, 255, 255}})
		        .set<Position>({(w / 2.0) - 16.0, (h / 2.0) - 16.0})
		        .set<Ball>({0.15, 0.15, 0, 0.15, 0.15});

//		// Set up collideables
		m_collideables.ai        = ai_paddle;
		m_collideables.player    = player_paddle;
		m_collideables.ball      = ball;

		m_input_system = new InputSystem(&m_window);
		m_ecs.system<Player, Position>("InputSystem")
                .kind(flecs::PreUpdate)
                .ctx(static_cast<void*>(m_input_system))
                .iter(inputSystem_process);

		m_ecs.system<Player, Position>("MoveSystemPlayer")
		        .iter(moveSystem_processPlayer);
        m_ecs.system<Ball, Position>("MoveSystemBall")
                .iter(moveSystem_processBall);

        m_ai_system = new AISystem(&m_window);
        m_ecs.system<AI, Position>("MoveSystemAI")
                .ctx(static_cast<void*>(m_ai_system))
                .iter(aiSystem_process);

        m_ecs.system<Ball, Position, Sprite>("CollisionSystem")
                .kind(flecs::PostUpdate)
                .ctx(static_cast<void*>(&m_collideables))
                .iter(collisionSystem_process);

        m_render_system = new RenderSystem(&m_window);
        m_ecs.system("RenderSystemFlush")
                .kind(flecs::PreStore)
                .ctx(static_cast<void*>(m_render_system))
                .iter(renderSystem_flush);
        m_ecs.system<Sprite, Position>("RenderSystem")
                .kind(flecs::OnStore)
                .ctx(static_cast<void*>(m_render_system))
                .iter(renderSystem_process);
        m_ecs.system("RenderSystemDraw")
                .kind(flecs::OnStore)
                .ctx(static_cast<void*>(m_render_system))
                .iter(renderSystem_draw);
	}

	Game::~Game() noexcept
	{
	    delete m_input_system;
	    delete m_render_system;
	    delete m_ai_system;
		SDL_Quit();
	}

	const int Game::run()
	{
		// 60 updates per second. We divide 1000 by 60 instead of 1 because sdl operates on milliseconds
		// not nanoseconds.
		const constexpr double dt = 1000.0 / 60.0;

		// This is a fixed-step gameloop.
		// See https://gafferongames.com/post/fix_your_timestep/
		// For an explanation.
		double time         = 0.0;
		double accumulator  = 0.0;
		double current_time = SDL_GetTicks();
		double new_time     = 0.0;
		double frame_time   = 0.0;

		while (m_window.is_open())
		{
			new_time     = SDL_GetTicks();
			frame_time   = new_time - current_time;
			current_time = new_time;

			accumulator += frame_time;

			while (accumulator >= dt)
			{

			    m_ecs.progress(accumulator);

				accumulator -= dt;
				time += dt;
			}

		}

        m_window.destroy();
        SDL_Delay(200);
		return EXIT_SUCCESS;
	}

} // namespace ep
