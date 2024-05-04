///
/// AISystem.cpp
/// FlecsPong
///
/// Refer to LICENSE.txt for more details.
///

#include <flecs.h>


#include "../components/AI.hpp"
#include "../components/Ball.hpp"
#include "../components/Position.hpp"
#include "../components/Sprite.hpp"

#include "AISystem.hpp"

namespace fp
{

AISystem::AISystem(Window* window)
{
    m_window = window;
}

void aiSystem_process(flecs::iter &it, AI *ai, Position *ai_pos)
{
    ai->m_x = ai_pos->m_x;
    ai->m_y = ai_pos->m_y;

//    AISystem * aisystem = static_cast<AISystem*>(it.ctx()); //TODO: use window size
    const auto& ball_pos = it.world().lookup("Ball").get<Position>()[0];
    const auto& ai_spr = it.world().lookup("AIPaddle").get<Sprite>()[0];
    if (ball_pos.m_y > (ai_pos->m_y+(ai_spr.m_height/2)))
    {
        ai_pos->m_y += 2.5;
    }
    else if (ball_pos.m_y < (ai_pos->m_y+(ai_spr.m_height/2)))
    {
        ai_pos->m_y -= 2.5;
    }

    // Lock to screen.
    if (ai_pos->m_y < 0.0)
    {
        ai_pos->m_y = 0.0;
    }
    else if (ai_pos->m_y > (480.0 - ai_spr.m_height)) // screen heigh - sprite heigh
    {
        ai_pos->m_y = (480.0 - ai_spr.m_height);
    }
}

} // namespace ep
