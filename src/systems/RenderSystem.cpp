///
/// RenderSystem.cpp
/// FlecsPong
///
/// Refer to LICENSE.txt for more details.
///

#include <flecs.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include "../components/Position.hpp"
#include "../components/Sprite.hpp"

#include "RenderSystem.hpp"

namespace fp
{
    RenderSystem::RenderSystem(Window *window)
    {
        m_window = window;
    }

	void RenderSystem::render(Window& window, flecs::world& world)
	{
		// First we retrieve a view of the entitys we want to process, specifying which components.
		// Then we iterate over each one and utilize the components.
	    auto render_query = world.query<Sprite, Position>();
	    render_query.each([&](Sprite& spr, Position& pos) {
	        if (spr.m_radius != 0)
              {
                  // We use SDL2_gfx to make drawing circles easier.
                  filledCircleRGBA(window.renderer(), static_cast<Sint16>(pos.m_x), static_cast<Sint16>(pos.m_y), spr.m_radius, spr.m_colour.r, spr.m_colour.g, spr.m_colour.b, spr.m_colour.a);
              }
              else
              {
                  // First we set the rectangle fill colour to that of the spritecomponents.
                  SDL_SetRenderDrawColor(window.renderer(), spr.m_colour.r, spr.m_colour.g, spr.m_colour.b, spr.m_colour.a);

                  // Then we create the actual rectangle.
                  const SDL_Rect draw_rect {static_cast<int>(pos.m_x), static_cast<int>(pos.m_y), spr.m_width, spr.m_height};

                  // Now the rectangle gets renderered with the appropriate colours and position data to the window.
                  SDL_RenderFillRect(window.renderer(), &draw_rect);
              }
	    });
	}

//static Window *s_window1;
//
//void renderSystem_init(Window *window)
//{
//    s_window1 = window;
//}

void renderSystem_flush(flecs::iter &it)
{
    // Flush renderer.
//    SDL_SetRenderDrawColor(s_window->renderer(), 0, 0, 0, SDL_ALPHA_OPAQUE);
//    SDL_RenderClear(s_window->renderer());
    RenderSystem * render = static_cast<RenderSystem*>(it.ctx());
//    Window * window = static_cast<Window*>(it.ctx());
    SDL_SetRenderDrawColor(render->m_window->renderer(), 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(render->m_window->renderer());
}

void renderSystem_process(flecs::iter &it, Sprite *spr, Position *pos)
{
    RenderSystem * render = static_cast<RenderSystem*>(it.ctx());
//    Window * window = static_cast<Window*>(it.ctx());
    if (spr->m_radius != 0)
      {
          // We use SDL2_gfx to make drawing circles easier.
          filledCircleRGBA(render->m_window->renderer(), static_cast<Sint16>(pos->m_x), static_cast<Sint16>(pos->m_y), spr->m_radius, spr->m_colour.r, spr->m_colour.g, spr->m_colour.b, spr->m_colour.a);
      }
      else
      {
          // First we set the rectangle fill colour to that of the spritecomponents.
          SDL_SetRenderDrawColor(render->m_window->renderer(), spr->m_colour.r, spr->m_colour.g, spr->m_colour.b, spr->m_colour.a);

          // Then we create the actual rectangle.
          const SDL_Rect draw_rect {static_cast<int>(pos->m_x), static_cast<int>(pos->m_y), spr->m_width, spr->m_height};

          // Now the rectangle gets renderered with the appropriate colours and position data to the window.
          SDL_RenderFillRect(render->m_window->renderer(), &draw_rect);
      }
}

void renderSystem_draw(flecs::iter &it)
{
    RenderSystem * render = static_cast<RenderSystem*>(it.ctx());
//    Window * window = static_cast<Window*>(it.ctx());
    SDL_RenderPresent(render->m_window->renderer());
}

} // namespace ep


