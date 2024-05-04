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

#include "InputSystem.hpp"

namespace fp
{


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

} // namespace ep
