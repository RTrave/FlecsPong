///
/// RenderSystem.cpp
/// FlecsPong
///
/// Refer to LICENSE.txt for more details.
///

#include <flecs.h>
#include <SDL2/SDL_render.h>
//#include <SDL2/SDL2_gfxPrimitives.h>

#include "../components/Position.hpp"
#include "../components/Sprite.hpp"
#include "../core/Window.hpp"

#include "RenderSystem.hpp"

namespace fp
{

RenderSystem::RenderSystem(Window *window)
{
    m_window = window;
}

void renderSystem_flush(flecs::iter &it)
{
    // Flush renderer.
    RenderSystem *render = static_cast<RenderSystem*>(it.ctx());
    SDL_SetRenderDrawColor(render->m_window->renderer(),
                           0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(render->m_window->renderer());
}

void renderSystem_process(flecs::iter &it, const Sprite *spr,
                          const Position *pos)
{
    RenderSystem *render = static_cast<RenderSystem*>(it.ctx());
    for (auto i : it)
    {
        // First we set the rectangle fill colour to that of the spritecomponents.
        SDL_SetRenderDrawColor(render->m_window->renderer(),
                spr[i].m_colour.r, spr[i].m_colour.g, spr[i].m_colour.b,
                spr[i].m_colour.a);
        if (spr[i].m_radius != 0)
        {
//            // We use SDL2_gfx to make drawing circles easier.
//            filledCircleRGBA(render->m_window->renderer(),
//                    static_cast<Sint16>(pos[i].m_x),
//                    static_cast<Sint16>(pos[i].m_y), spr[i].m_radius,
//                    spr[i].m_colour.r, spr[i].m_colour.g, spr[i].m_colour.b,
//                    spr[i].m_colour.a);

            // Then we create the actual rectangle.
            const SDL_Rect draw_rect
            {
                static_cast<int>(pos[i].m_x - spr[i].m_radius),
                static_cast<int>(pos[i].m_y - spr[i].m_radius),
                spr[i].m_radius*2, spr[i].m_radius*2 };

            // Now the rectangle gets renderered with the appropriate colours and position data to the window.
            SDL_RenderFillRect(render->m_window->renderer(), &draw_rect);
        }
        else
        {

            // Then we create the actual rectangle.
            const SDL_Rect draw_rect
            { static_cast<int>(pos[i].m_x), static_cast<int>(pos[i].m_y),
                    spr[i].m_width, spr[i].m_height };

            // Now the rectangle gets renderered with the appropriate colours and position data to the window.
            SDL_RenderFillRect(render->m_window->renderer(), &draw_rect);
        }
    }
}

void renderSystem_draw(flecs::iter &it)
{
    RenderSystem *render = static_cast<RenderSystem*>(it.ctx());
    SDL_RenderPresent(render->m_window->renderer());
}

} // namespace ep

