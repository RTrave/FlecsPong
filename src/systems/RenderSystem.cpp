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
#include "../components/RenderPosition.hpp"
#include "../components/Sprite.hpp"
#include "../core/Window.hpp"

#include "RenderSystem.hpp"

namespace fp
{

RenderSystem::RenderSystem(Window *window, bool is_threaded)
{
    m_window = window;
    m_threaded = is_threaded;
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
                          const Position *pos,
                          const RenderPosition *rpos)
{
    RenderSystem *render = static_cast<RenderSystem*>(it.ctx());
    SDL_Rect draw_rect;
    for (auto i : it)
    {
        // Initialize draw Rect
        if(render->isThreaded())
        {
            draw_rect.x = rpos[i].m_x;
            draw_rect.y = rpos[i].m_y;
        }
        else
        {
            draw_rect.x = pos[i].m_x;
            draw_rect.y = pos[i].m_y;
        }
        draw_rect.w = spr[i].m_width;
        draw_rect.h = spr[i].m_height;

        // First we set the rectangle fill colour to that of the spritecomponents.
        SDL_SetRenderDrawColor(render->m_window->renderer(),
                spr[i].m_colour.r, spr[i].m_colour.g, spr[i].m_colour.b,
                spr[i].m_colour.a);

        // Now the rectangle gets renderered with the appropriate colours and position data to the window.
        SDL_RenderFillRect(render->m_window->renderer(), &draw_rect);
    }
}

void renderSystem_draw(flecs::iter &it)
{
    RenderSystem *render = static_cast<RenderSystem*>(it.ctx());
    SDL_RenderPresent(render->m_window->renderer());
}

} // namespace ep

