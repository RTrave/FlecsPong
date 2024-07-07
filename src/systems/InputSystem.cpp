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
#include "../core/Game.hpp"

#include "InputSystem.hpp"

namespace fp {

InputSystem *input_system;

InputSystem::InputSystem(Game *game, Window *window)
{
    m_game = game;
    m_window = window;
    m_azerty = false;
    m_queryPlayer =
            game->m_ecs.query_builder<const Player, Paddle, Velocity>()
//                .filter()
                .build();
    input_system = this;
}

int balln_id = 1;

bool InputSystem::processPlayerEvent(flecs::iter &it)
{
    bool event_ok = false;
    while (it.next())
    {
//            auto player = it.field<const Player>(0);
        auto paddle = it.field<Paddle>(1);
        auto velocity = it.field<Velocity>(2);

        for (auto i : it)
        {
            switch (input_system->m_window->m_event.type)
            {
            case SDL_KEYDOWN:

                // Player1 controls
                if (it.entity(i).name() == "Player1")
                {
                    if (input_system->isAzerty())
                    {
                        switch (input_system->m_window->m_event.key.keysym.sym)
                        {
                        case SDLK_z:
                            event_ok = true;
                            paddle[i].m_movement = Paddle::MoveDirection::NORTH;
                            velocity[i].m_vel_y = -paddle[i].m_velocity;
                            break;

                        case SDLK_s:
                            event_ok = true;
                            paddle[i].m_movement = Paddle::MoveDirection::SOUTH;
                            velocity[i].m_vel_y = paddle[i].m_velocity;
                            break;
                        }
                    }
                    else
                    {
                        switch (input_system->m_window->m_event.key.keysym.sym)
                        {
                        case SDLK_w:
                            event_ok = true;
                            paddle[i].m_movement = Paddle::MoveDirection::NORTH;
                            velocity[i].m_vel_y = -paddle[i].m_velocity;
                            break;

                        case SDLK_s:
                            event_ok = true;
                            paddle[i].m_movement = Paddle::MoveDirection::SOUTH;
                            velocity[i].m_vel_y = paddle[i].m_velocity;
                            break;
                        }
                    }
                }
                // Player2 controls
                else if (it.entity(i).name() == "Player2")
                {
                    switch (input_system->m_window->m_event.key.keysym.sym)
                    {
                    case SDLK_UP:
                        event_ok = true;
                        paddle[i].m_movement = Paddle::MoveDirection::NORTH;
                        velocity[i].m_vel_y = -paddle[i].m_velocity;
                        break;

                    case SDLK_DOWN:
                        event_ok = true;
                        paddle[i].m_movement = Paddle::MoveDirection::SOUTH;
                        velocity[i].m_vel_y = paddle[i].m_velocity;
                        break;
                    }
                }
                break;
            case SDL_KEYUP:
                // Player1 controls
                if (it.entity(i).name() == "Player1")
                {
                    if (input_system->m_window->m_event.key.keysym.sym == SDLK_w
                            || input_system->m_window->m_event.key.keysym.sym
                                    == SDLK_s
                            || input_system->m_window->m_event.key.keysym.sym
                                    == SDLK_z)
                    {
                        event_ok = true;
                        paddle[i].m_movement = Paddle::MoveDirection::STOPPED;
                        velocity[i].m_vel_y = 0.0;
                    }
                }
                // Player2 controls
                else if (it.entity(i).name() == "Player2")
                {
                    if (input_system->m_window->m_event.key.keysym.sym
                            == SDLK_UP
                            || input_system->m_window->m_event.key.keysym.sym
                                    == SDLK_DOWN)
                    {
                        event_ok = true;
                        paddle[i].m_movement = Paddle::MoveDirection::STOPPED;
                        velocity[i].m_vel_y = 0.0;
                    }
                }
                break;
            }
        }

    }
    return event_ok;
}

void InputSystem::processGlobalEvent()
{
    switch (input_system->m_window->m_event.type)
    {
    case SDL_QUIT:
        input_system->m_window->close();
        break;

    case SDL_KEYDOWN:
        switch (input_system->m_window->m_event.key.keysym.sym)
        {
        case SDLK_n: // Reset game
            input_system->m_game->reset();
            input_system->m_game->createKBalls(1);
            break;

        case SDLK_b: // Create a new ball
        {
            input_system->m_game->createBall();
            break;
        }
        case SDLK_k: // Create nb balls
            input_system->m_game->createKBalls(50000);
            break;

        case SDLK_v: // Switch PVP/Player for Paddle2
            input_system->m_game->switchPVP();
            break;

        case SDLK_f: // Switch PVP/Player for Paddle2
            input_system->switchAzerty();
            break;

        case SDLK_ESCAPE: // Quit FlecsPong
            input_system->m_window->close();
            break;
        }
        break;
    }
}

bool event_ok = false;

void inputSystem_process(flecs::iter &iter)
{
    event_ok = false;

    // Process all user and system events.
    while (SDL_PollEvent(&input_system->m_window->m_event) != 0)
    {

        if (input_system->m_game->m_multithreaded)
        {
            input_system->m_queryPlayer
                .iter(input_system->m_game->m_thr_stage_0)
                .run([](flecs::iter &it) {

                event_ok = input_system->processPlayerEvent(it);

            });
        }
        else
        {
            input_system->m_queryPlayer
                .run([](flecs::iter &it)
            {

                event_ok = input_system->processPlayerEvent(it);

            });
        }

        // General controls
        if (!event_ok)
            input_system->processGlobalEvent();

    }
}

} // namespace ep
