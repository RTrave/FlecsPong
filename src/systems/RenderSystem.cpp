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

namespace fp {

RenderSystem::RenderSystem(Window *window, bool is_threaded)
{
    m_window = window;
    m_threaded = is_threaded;
}

void renderSystem_flush(flecs::iter &it)
{
    // Flush renderer.
    RenderSystem *render = static_cast<RenderSystem*>(it.ctx());
    SDL_SetRenderDrawColor(render->m_window->renderer(), 0, 0, 0,
            SDL_ALPHA_OPAQUE);
    SDL_RenderClear(render->m_window->renderer());
}

void renderSystem_process(flecs::iter &it, size_t i, const Sprite &spr,
        const Position &pos)
{
    RenderSystem *render = static_cast<RenderSystem*>(it.ctx());
    SDL_Rect draw_rect;
    if (pos.m_x < 0.0 || pos.m_x > (640.0 - 16.0))
        return;

    // Initialize draw Rect
    draw_rect.x = pos.m_x;
    draw_rect.y = pos.m_y;
    draw_rect.w = spr.m_width;
    draw_rect.h = spr.m_height;

    // First we set the rectangle fill colour to that of the spritecomponents.
    SDL_SetRenderDrawColor(render->m_window->renderer(), spr.m_colour.r,
            spr.m_colour.g, spr.m_colour.b, spr.m_colour.a);

    // Now the rectangle gets renderered with the appropriate colours and position data to the window.
    SDL_RenderFillRect(render->m_window->renderer(), &draw_rect);
}

void renderSystem_draw(flecs::iter &it)
{
    RenderSystem *render = static_cast<RenderSystem*>(it.ctx());
    SDL_RenderPresent(render->m_window->renderer());
}

} // namespace ep

