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
}

void inputSystem_process(flecs::iter &it, Player *player, Paddle *paddle, Velocity *velocity)
{
    InputSystem * input = static_cast<InputSystem*>(it.ctx());
    auto ball = it.world().lookup("Ball1");
    const auto& ball_c = ball.get<Ball>()[0];
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
                        paddle->m_movement = Paddle::MoveDirection::NORTH;
                        velocity->m_vel_y = -paddle->m_velocity;
                        break;

                    case SDLK_s:
                        paddle->m_movement = Paddle::MoveDirection::SOUTH;
                        velocity->m_vel_y = paddle->m_velocity;
                        break;

                    case SDLK_n:
                        ball.set<Velocity>({ball_c.m_initial_vel_x, ball_c.m_initial_vel_y})
                        .set<Position>({(640 / 2.0) - 16.0, (480 / 2.0) - 16.0});
                        break;

                    case SDLK_b:
                        input->m_game->createBall();
                        break;

                    case SDLK_ESCAPE:
                        input->m_window->close();
                        break;
                }
                break;

            case SDL_KEYUP:
                if (input->m_window->m_event.key.keysym.sym == SDLK_w || input->m_window->m_event.key.keysym.sym == SDLK_s)
                {
                    paddle->m_movement = Paddle::MoveDirection::STOPPED;
                    velocity->m_vel_y = 0.0;
                }
                break;
        }
    }
}

} // namespace ep
