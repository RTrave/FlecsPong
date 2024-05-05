///
/// Game.cpp
/// FlecsPong
///
/// Refer to LICENSE.txt for more details.
///

#include <SDL2/SDL.h>

#include "../components/All.hpp"
#include "../systems/InputSystem.hpp"

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

		printf("FlecsPong\n");
		printf("Player1: W/S keys to move paddle\n");
        printf("type n to reset\n");
        printf("type b to add a new ball\n");
        printf("type v to switch Player2 AI\n\n");

		// Here, we are creating the entities using Flecs and
		//attaching the relevant components and tags.
		auto player1 = m_ecs.entity("Player1")
		        .set<Position>({20.0, 20.0})
		        .set<Velocity>({0.0, 0.0})
                .set([](Sprite &spr, Paddle &pad)
            {
                spr.m_width = 12;
                spr.m_height = 96;
                spr.m_colour = SDL_Colour{255, 255, 255, 255};
                spr.m_radius = 0;
                pad.m_velocity = 0.2;
                pad.m_movement = Paddle::MoveDirection::STOPPED;
            });
        auto player2 = m_ecs.entity("Player2")
                .set<Position>({w - 32.0, 20.0})
                .set<Velocity>({0.0, 0.0})
                .set([](Sprite &spr, Paddle &pad)
            {
                spr.m_width = 12;
                spr.m_height = 96;
                spr.m_colour = SDL_Colour{255, 255, 255, 255};
                spr.m_radius = 0;
                pad.m_velocity = 0.2;
                pad.m_movement = Paddle::MoveDirection::STOPPED;
            });

        // Then we add Player or AI component
        player1.add<Player>();
        player2.add<AI>();

        createBall();
//        auto ball = m_ecs.entity("Ball")
//                .set<Position>({(w / 2.0) - 16.0, (h / 4.0) - 16.0})
//                .set<Velocity>({0.25, 0.25})
//                .set<Ball>({0, 0.25, 0.25})
//                .set([](Sprite &spr)
//            {
//                spr.m_width = 0;
//                spr.m_height = 8;
//                spr.m_colour = SDL_Colour{255, 255, 255, 255};
//                spr.m_radius = 8;
//            });

//		// Set up collideables
		m_collideables.player1 = player1;
		m_collideables.player2 = player2;
        m_collideables.score_player1 = 0;
        m_collideables.score_player2 = 0;

		// Set up Input and AI systems (updating Velocity)
		m_input_system = new InputSystem(this, &m_window);
		m_ecs.system<Player, Paddle, Velocity>("InputSystem")
                .kind(flecs::PreUpdate)
                .ctx(static_cast<void*>(m_input_system))
//                .interval(10.0)
//                .rate(2)
                .iter(inputSystem_process);
        m_ai_system = new AISystem(&m_window);
        m_ecs.system<AI, Paddle, Velocity, Position, Sprite>("AISystem")
                .ctx(static_cast<void*>(m_ai_system))
                .interval(10.0)
//                .rate(10)
                .iter(aiSystem_process);

//		m_ecs.system<Player, Position>("MoveSystemPlayer")
//		        .iter(moveSystem_processPlayer);
//        m_ecs.system<Ball, Position>("MoveSystemBall")
//                .iter(moveSystem_processBall);
        m_ecs.system<Velocity, Position, Sprite>("MovePlayer")
                .iter(moveSystem_process);


        m_ecs.system<Ball, Position, Velocity, Sprite>("CollisionSystem")
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
        printf("\nFinal score:\nPlayer1 %d-%d Player2\n",
                m_collideables.score_player1, m_collideables.score_player2);
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

	int ball_id = 1;

flecs::entity Game::createBall()
{
    std::string ball_name("Ball");
    ball_name += std::to_string(ball_id++);
    printf("New ball: %s\n", ball_name.c_str());
    return m_ecs.entity(ball_name.c_str())
            .set<Position>({(640 / 2.0) - 16.0, (480 / 4.0) - 16.0})
            .set<Velocity>({0.20, 0.20})
            .set<Ball>({0, 0.20, 0.20})
            .set([](Sprite &spr)
        {
            spr.m_width = 0;
            spr.m_height = 8;
            spr.m_colour = SDL_Colour{255, 255, 255, 255};
            spr.m_radius = 8;
        });
}

void Game::switchPVP()
{
    auto p2 = m_ecs.lookup("Player2");
    if(p2.has<AI>())
    {
        p2.remove<AI>().add<Player>();
        printf("Paddler2 now controlled by player (UP/DOWN keys)\n");
    }
    else
    {
        p2.remove<Player>().add<AI>();
        printf("Paddle2 now controlled by AI\n");
    }
}

} // namespace ep
