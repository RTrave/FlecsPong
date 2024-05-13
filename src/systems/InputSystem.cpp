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

namespace fp
{


InputSystem::InputSystem(Game* game, Window *window)
{
    m_game = game;
    m_window = window;
    m_azerty = false;
}

void inputSystem_process(flecs::iter &it, const Player *player, Paddle *paddle, Velocity *velocity)
{
    InputSystem * input_system = static_cast<InputSystem*>(it.ctx());

    // Process all user and system events.
    while (SDL_PollEvent(&input_system->m_window->m_event) != 0)
    {
        bool event_ok = false;
        for (auto i : it)
        {
            switch (input_system->m_window->m_event.type)
            {
            case SDL_KEYDOWN:

                // Player1 controls
                if(it.entity(i).name()=="Player1")
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
                else if(it.entity(i).name()=="Player2")
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
                            || input_system->m_window->m_event.key.keysym.sym == SDLK_s
                            || input_system->m_window->m_event.key.keysym.sym == SDLK_z)
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
                            || input_system->m_window->m_event.key.keysym.sym == SDLK_DOWN)
                    {
                        event_ok = true;
                        paddle[i].m_movement = Paddle::MoveDirection::STOPPED;
                        velocity[i].m_vel_y = 0.0;
                    }
                }
                break;
            }
            if(event_ok) return;
        }
        // General controls
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
                        input_system->m_game->createBall();
                        break;

                    case SDLK_b: // Create a new ball
                        input_system->m_game->createBall();
                        break;

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
}

} // namespace ep
