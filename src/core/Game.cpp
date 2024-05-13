///
/// Game.cpp
/// FlecsPong
///
/// Refer to LICENSE.txt for more details.
///

#include <SDL2/SDL.h>

#include "../components/All.hpp"
#include "../systems/InputSystem.hpp"
#include "../systems/AISystem.hpp"
#include "../systems/MoveSystem.hpp"
#include "../systems/RenderSystem.hpp"

#include "../Log.hpp"

#include "Game.hpp"

namespace fp
{
Game::Game(std::string_view title, const int w, const int h,
           std::uint32_t flags)
                   : m_window{ }
{
    // Setup SDL2 and stuff.
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0)
    {
        throw_sdl2_exception("Failed to init SDL2.");
    }

    m_window.create(title, w, h, flags);

    printf("FlecsPong\n");
    printf("Player1: (W/S) keys to move paddle\n");
    printf("type f to switch to azerty keyboard (Z/S)\n");
    printf("type n to reset\n");
    printf("type b to add a new ball\n");
    printf("type v to switch Player2 AI (UP/DOWN)\n\n");

    // Here, we are creating the entities using Flecs and
    //attaching the relevant components and tags.
    auto player1 = m_ecs.entity("Player1")
            .set<Position>({ 20.0, 240.0 })
            .set<Velocity>({ 0.0, 0.0 })
            .set([](Sprite &spr, Paddle &pad)
    {
        spr.m_width = 12;
        spr.m_height = 96;
        spr.m_colour = SDL_Colour
        {   255, 255, 255, 255};
        spr.m_radius = 0;
        pad.m_velocity = 0.2;
        pad.m_movement = Paddle::MoveDirection::STOPPED;
    });
    auto player2 = m_ecs.entity("Player2")
            .set<Position>({ w - 32.0, 240.0 })
            .set<Velocity>({ 0.0, 0.0 })
            .set([](Sprite &spr, Paddle &pad)
    {
        spr.m_width = 12;
        spr.m_height = 96;
        spr.m_colour = SDL_Colour
        {   255, 255, 255, 255};
        spr.m_radius = 0;
        pad.m_velocity = 0.2;
        pad.m_movement = Paddle::MoveDirection::STOPPED;
    });

    // Then we add Player or AI component
    player1.add<Player>();
    player2.add<AI>();

    // Create the first ball
    createBall();

    // Set up collideables
    m_collideables.player1 = player1;
    m_collideables.player2 = player2;
    m_collideables.score_player1 = 0;
    m_collideables.score_player2 = 0;

    // Set up Input and AI systems (updating Velocity)
    m_input_system = new InputSystem(this, &m_window);
    m_ecs.system<const Player, Paddle, Velocity>("InputSystem")
            .kind(flecs::PreUpdate)
            .ctx(static_cast<void*>(m_input_system))
            .iter(inputSystem_process);
    m_ai_system = new AISystem(this, &m_window);
    m_ecs.system<const AI, const Paddle, Velocity, const Position, const Sprite>("AISystem")
            .ctx(static_cast<void*>(m_ai_system))
            .iter(aiSystem_process);

    // Set up Move system (updating Position)
    m_ecs.system<Velocity, Position, const Sprite>("MoveSystem")
            .iter(moveSystem_process);

    // Set up Collision system (updating Movement and Position), and
    // processing scores
    m_ecs.system<Ball, Position, Velocity, const Sprite>("CollisionSystem")
            .kind(flecs::PostUpdate)
            .ctx(static_cast<void*>(&m_collideables))
            .iter(collisionSystem_process);

    // Setup Render Systems, Flush and Draw are tasks
    m_render_system = new RenderSystem(&m_window);
    m_ecs.system("RenderSystemFlush")
            .kind(flecs::PreStore)
            .ctx(static_cast<void*>(m_render_system))
            .iter(renderSystem_flush);
    m_ecs.system<const Sprite, const Position>("RenderSystem")
            .kind(flecs::OnStore)
            .ctx(static_cast<void*>(m_render_system))
            .iter(renderSystem_process);
    m_ecs.system("RenderSystemDraw")
            .kind(flecs::OnStore)
            .ctx(static_cast<void*>(m_render_system)).iter(renderSystem_draw);
}

Game::~Game() noexcept
{
    // End game, display scores and delete members system contexts
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
    m_ecs.set_target_fps(240.0);

    double time = SDL_GetTicks();
    double old_time = 0.0;
    double frame_time = dt;
    double fps_time = SDL_GetTicks();
    double fps_ticks = 0;

    while (m_window.is_open())
    {

        // Call the system processing
        m_ecs.progress(frame_time);

        // Create balls if request
        if(m_ballsToCreate) {
            int i = 0;
            while(m_ballsToCreate and i<=1000) {
                createBall();
                m_ballsToCreate--;
                i++;
            }
            if(!m_ballsToCreate)
                printf("K Balls created\n");
        }

        fps_ticks++;
        if(fps_ticks!=0 and (SDL_GetTicks()-fps_time)>1000) {
            printf("FPS: %f\n",
                    fps_ticks*1000/(SDL_GetTicks()-fps_time));
            fps_time = SDL_GetTicks();
            fps_ticks = 0;
        }

        // Check for delay to wait for before next frame
        old_time = time;
        time = SDL_GetTicks();
        frame_time = time - old_time;
        if (dt > frame_time)
            SDL_Delay(dt - frame_time);
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
//    printf("New ball: %s\n", ball_name.c_str());
    return m_ecs.entity(ball_name.c_str())
            .set<Position>({ (640 / 2.0) - 8.0, (480 / 2.0) - 8.0 })
            .set<Velocity>({ 0.25, 0.25 })
            .set<Ball>({ 0, 0.25, 0.25 })
            .set([](Sprite &spr)
    {
        spr.m_width = 0;
        spr.m_height = 8;
        spr.m_colour = SDL_Colour
        {   255, 255, 255, 255};
        spr.m_radius = 8;
    });
}

void Game::createKBalls(int nb)
{
    printf("Create %d balls\n", nb);
    m_ballsToCreate = nb;
}

void Game::reset()
{
    // Display and reset scores
    printf("\nFinal score:\nPlayer1 %d-%d Player2\n",
            m_collideables.score_player1, m_collideables.score_player2);
    m_collideables.score_player1 = 0;
    m_collideables.score_player2 = 0;

    // Destroy all Ball entities
    int nb = 0;
    m_ecs.each<Ball>([&](flecs::entity bb, Ball &tball)
    {
        bb.destruct();
        nb++;
    });
    printf("\nGame restarted (%d balls destroyed)\n", nb);
}

void Game::switchPVP()
{
    // If AI tag is present, remove it and add Player tag
    auto p2 = m_ecs.lookup("Player2");
    if (p2.has<AI>())
    {
        p2.remove<AI>().add<Player>();
        printf("Paddler2 now controlled by player (UP/DOWN keys)\n");
    }
    // If Player tag is present, remove it and add AI tag
    else
    {
        p2.remove<Player>().add<AI>();
        printf("Paddle2 now controlled by AI\n");
    }
}

} // namespace ep
