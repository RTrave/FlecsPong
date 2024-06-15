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
#include "../systems/SyncSystem.hpp"

#include "../Log.hpp"

#include "Game.hpp"

namespace fp
{
Game::Game(std::string_view title, const int w, const int h,
           std::uint32_t flags)
                   : m_window{ }
{
    m_systemMutex = SDL_CreateMutex();
    m_systemFlag = THREAD_STATE_OFF;

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
    m_player1 = m_ecs.entity("Player1")
            .set<Position>({ 20.0, 240.0 })
            .set<RenderPosition>({ 20.0, 240.0 })
            .set<Velocity>({ 0.0, 0.0 })
            .set([](Sprite &spr, Paddle &pad)
    {
        spr.m_width = 12;
        spr.m_height = 96;
        spr.m_colour = SDL_Colour
        {   255, 255, 255, 255};
        pad.m_velocity = 0.2;
        pad.m_movement = Paddle::MoveDirection::STOPPED;
    });
    m_player2 = m_ecs.entity("Player2")
            .set<Position>({ w - 32.0, 240.0 })
            .set<RenderPosition>({ w - 32.0, 240.0 })
            .set<Velocity>({ 0.0, 0.0 })
            .set([](Sprite &spr, Paddle &pad)
    {
        spr.m_width = 12;
        spr.m_height = 96;
        spr.m_colour = SDL_Colour
        {   255, 255, 255, 255};
        pad.m_velocity = 0.2;
        pad.m_movement = Paddle::MoveDirection::STOPPED;
    });

    // Then we add Player or AI component
    m_player1.add<Player>();
    m_player2.add<AI>();

    // Create the first ball
    createBall();
}

Game::~Game() noexcept
{
    if(m_multithreaded)
    {
        SDL_LockMutex(m_systemMutex);
        m_systemFlag = THREAD_STATE_STOPING;
        while (m_systemFlag != THREAD_STATE_OFF)
        {
            SDL_UnlockMutex(m_systemMutex);
            SDL_Delay(50);
            SDL_LockMutex(m_systemMutex);
        }
        SDL_Delay(50);
        SDL_UnlockMutex(m_systemMutex);
        delete m_sync_system;
    }
    SDL_DestroyMutex(m_systemMutex);
    // End game, display scores and delete members system contexts
    printf("\nFinal score:\nPlayer1 %d-%d Player2\n",
            m_collideables.score_player1, m_collideables.score_player2);
    delete m_input_system;
    delete m_render_system;
    delete m_ai_system;
    SDL_Quit();
}


void Game::systemsInit()
{
    // Set up collideables
    m_collideables.player1 = m_player1;
    m_collideables.player2 = m_player2;
    m_collideables.score_player1 = 0;
    m_collideables.score_player2 = 0;

    // Set up Input and AI systems (updating Velocity)
    m_input_system = new InputSystem(this, &m_window);
    m_inputsys = m_ecs.system<const Player, Paddle, Velocity>("InputSystem")
            .kind(flecs::PreUpdate)
            .ctx(static_cast<void*>(m_input_system))
            .iter(inputSystem_process);

    m_ai_system = new AISystem(this, &m_window);
    m_aisys = m_ecs.system<const AI, const Paddle, Velocity, const Position, const Sprite>("AISystem")
            .ctx(static_cast<void*>(m_ai_system))
            .iter(aiSystem_process);

    // Set up Move system (updating Position)
    m_movesys = m_ecs.system<Velocity, Position, const Sprite>("MoveSystem")
            .iter(moveSystem_process);

    // Set up Collision system (updating Movement and Position), and
    // processing scores
    m_collisionsys = m_ecs.system<Ball, Position, Velocity, const Sprite>("CollisionSystem")
            .kind(flecs::PostUpdate)
            .ctx(static_cast<void*>(&m_collideables))
            .iter(collisionSystem_process);
    // Setup Render Systems, Flush and Draw are tasks
    m_render_system = new RenderSystem(&m_window, false);
    m_rendersys_flush = m_ecs.system("RenderSystemFlush")
            .kind(flecs::PreStore)
            .ctx(static_cast<void*>(m_render_system))
            .iter(renderSystem_flush);
    m_rendersys = m_ecs.system<const Sprite, const Position, const RenderPosition>("RenderSystem")
            .kind(flecs::OnStore)
            .ctx(static_cast<void*>(m_render_system))
            .iter(renderSystem_process);
    m_rendersys_draw = m_ecs.system("RenderSystemDraw")
            .kind(flecs::OnStore)
            .ctx(static_cast<void*>(m_render_system))
            .iter(renderSystem_draw);
}

void Game::systemsInit_MT()
{
    m_ecs.set_stage_count(5);
    m_ecs.set_threads(2);


    // Set up collideables
    m_collideables.player1 = m_player1;
    m_collideables.player2 = m_player2;
    m_collideables.score_player1 = 0;
    m_collideables.score_player2 = 0;

    // Set up Input and AI systems (updating Velocity)
    m_input_system = new InputSystem(this, &m_window);
    m_inputsys = m_ecs.system<const Player, Paddle, Velocity>("InputSystem")
//                .kind(flecs::PreUpdate)
            .ctx(static_cast<void*>(m_input_system))
            .iter(inputSystem_process);

    m_ai_system = new AISystem(this, &m_window);
    m_aisys = m_ecs.system<const AI, const Paddle, Velocity, const Position, const Sprite>("AISystem")
            .ctx(static_cast<void*>(m_ai_system))
            .iter(aiSystem_process);

    // Set up Move system (updating Position)
    m_movesys = m_ecs.system<Velocity, Position, const Sprite>("MoveSystem")
        .multi_threaded()
        .iter(moveSystem_process);

    // Set up Collision system (updating Movement and Position), and
    // processing scores
    m_collisionsys = m_ecs.system<Ball, Position, Velocity, const Sprite>("CollisionSystem")
//                .kind(flecs::PostUpdate)
            .ctx(static_cast<void*>(&m_collideables))
            .multi_threaded()
            .iter(collisionSystem_process);

    m_sync_system = new SyncSystem();
    m_syncsys = m_ecs.system<const Position, RenderPosition>("SyncSystem")
//                .kind(flecs::PreStore)
            .ctx(static_cast<void*>(m_sync_system))
            .multi_threaded()
            .iter(syncSystem_process);

    // Setup Render Systems, Flush and Draw are tasks
    m_render_system = new RenderSystem(&m_window, true);
    m_rendersys_flush = m_ecs.system("RenderSystemFlush")
//                .kind(flecs::PreStore)
            .ctx(static_cast<void*>(m_render_system))
            .iter(renderSystem_flush);
    m_rendersys = m_ecs.system<const Sprite, const Position, const RenderPosition>("RenderSystem")
//                .kind(flecs::OnStore)
            .ctx(static_cast<void*>(m_render_system))
            .iter(renderSystem_process);
    m_rendersys_draw = m_ecs.system("RenderSystemDraw")
//                .kind(flecs::OnStore)
            .ctx(static_cast<void*>(m_render_system)).iter(renderSystem_draw);

//    m_systemMutex = SDL_CreateMutex();
//    m_systemFlag = THREAD_STATE_OFF;
}

int threadSystem(void *game)
{
    Game *game_t = static_cast<Game*>(game);
    SDL_Delay(50);
    game_t->threadLoop();
    return 0;
}

void Game::threadLoop()
{
    SDL_LockMutex(m_systemMutex);
    printf("System Thread launch\n");
    m_systemFlag = THREAD_STATE_ON;
    double frame_time;
    m_thr_stage_1 = m_ecs.get_stage(1);
    while (m_systemFlag == THREAD_STATE_ON)
    {
        while(!m_drawnFlag && m_systemFlag == THREAD_STATE_ON)
        {
            SDL_UnlockMutex(m_systemMutex);
            SDL_Delay(1);
            SDL_LockMutex(m_systemMutex);
        }

        frame_time = m_frame_time;
        SDL_UnlockMutex(m_systemMutex);
        m_aisys.run(frame_time).stage(m_thr_stage_1);
//        m_movesys.run(frame_time).stage(thr_stage_1);
        m_movesys.run_worker(0,2,frame_time).stage(m_thr_stage_1);
//        m_syncsys.run_worker(0,2,frame_time).stage(m_thr_stage_1);
//        m_collisionsys.run(frame_time).stage(thr_stage_1);
        m_collisionsys.run_worker(0,2,frame_time).stage(m_thr_stage_1);
        SDL_LockMutex(m_systemMutex);
        m_drawnFlag = false;
    }
    m_systemFlag = THREAD_STATE_OFF;
    SDL_UnlockMutex(m_systemMutex);
}

const int Game::run()
{
    systemsInit();

    // 60 updates per second. We divide 1000 by 60 instead of 1 because sdl operates on milliseconds
    // not nanoseconds.
    const constexpr double dt = 1000.0 / 60.0;
//    m_ecs.set_target_fps(240.0);

    double time = SDL_GetTicks();
    double old_time = 0.0;
    double frame_time = dt/2;
    double fps_time = SDL_GetTicks();
    double fps_ticks = 0;

    while (m_window.is_open())
    {
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

        // Call the system processing
        m_ecs.progress(frame_time);


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
            SDL_Delay(1);
//            SDL_Delay(dt - frame_time);
    }

    m_window.destroy();
    SDL_Delay(200);
    return EXIT_SUCCESS;
}

const int Game::run_MT()
{
    m_multithreaded = true;
    systemsInit_MT();

    // 60 updates per second. We divide 1000 by 60 instead of 1 because sdl operates on milliseconds
    // not nanoseconds.
    const constexpr double dt = 1000.0 / 60.0;
//    m_ecs.set_target_fps(240.0);

    double time = SDL_GetTicks();
    double old_time = 0.0;
    double frame_time = dt/2;
    double fps_time = SDL_GetTicks();
    double fps_ticks = 0;

    m_thr_stage_0 = m_ecs.get_stage(0);
    m_frame_time = dt/2;
    m_drawnFlag = false;

    SDL_LockMutex(m_systemMutex);
    m_systemThread = SDL_CreateThread(threadSystem,
            "SystemThread",
            this);
    while (m_systemFlag != THREAD_STATE_ON)
    {
        SDL_UnlockMutex(m_systemMutex);
        SDL_Delay(5);
        SDL_LockMutex(m_systemMutex);
    }
    SDL_UnlockMutex(m_systemMutex);

    while (m_window.is_open())
    {
        SDL_LockMutex(m_systemMutex);
        while(m_drawnFlag)
        {
            SDL_UnlockMutex(m_systemMutex);
            SDL_Delay(1);
            SDL_LockMutex(m_systemMutex);
        }
        SDL_UnlockMutex(m_systemMutex);

        // Create balls if request
        if(m_ballsToCreate) {
            int i = 0;
            while(m_ballsToCreate and i<=1000) {
                SDL_LockMutex(m_systemMutex);
                createBall();
                SDL_UnlockMutex(m_systemMutex);
                m_ballsToCreate--;
                i++;
            }
            if(!m_ballsToCreate)
                printf("K Balls created\n");
        }

        m_inputsys.run(frame_time).stage(m_thr_stage_0);
//      m_syncsys.run(m_frame_time).stage(m_thr_stage_0);
        m_syncsys.run_worker(0,2,m_frame_time).stage(m_thr_stage_0);
        SDL_LockMutex(m_systemMutex);
        m_drawnFlag = true;
        m_frame_time = frame_time;
        SDL_UnlockMutex(m_systemMutex);
        m_rendersys_flush.run(frame_time);
        m_rendersys.run(frame_time).stage(m_thr_stage_0);
        m_rendersys_draw.run(frame_time);

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
            SDL_Delay(1);
//            SDL_Delay(dt - frame_time);
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
            .set<RenderPosition>({ (640 / 2.0) - 8.0, (480 / 2.0) - 8.0 })
            .set<Velocity>({ 0.25, 0.25 })
            .set<Ball>({ 0, 0.25, 0.25 })
            .set([](Sprite &spr)
    {
        spr.m_width = 16;
        spr.m_height = 16;
        spr.m_colour = SDL_Colour
        {   255, 255, 255, 255};
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
