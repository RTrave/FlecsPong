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
	void AISystem::update(const double time, flecs::world& ecs)
	{
		// Center the ai to the ball so it is always in the right position.
		// we want to increase or decrese ai position based on if the ball is above or below it.
		// so something like:
		/*
			if ball position is above ai:
				move ai smoothly up towards ball so middle of ai paddle is at ball pos.
		*/

		// Takes advantage of the fact that there is only 1 AI and 1 Ball.
	    auto& ai_pos = ecs.lookup("AIPaddle").get_mut<Position>()[0];
        const auto& ai_spr = ecs.lookup("AIPaddle").get<Sprite>()[0];
        const auto& ball_pos = ecs.lookup("Ball").get<Position>()[0];
        if (ball_pos.m_y > (ai_pos.m_y+(ai_spr.m_height/2)))
        {
            ai_pos.m_y += 2.5;
        }
        else if (ball_pos.m_y < (ai_pos.m_y+(ai_spr.m_height/2)))
        {
            ai_pos.m_y -= 2.5;
        }

        // Lock to screen.
        if (ai_pos.m_y < 0.0)
        {
            ai_pos.m_y = 0.0;
        }
        else if (ai_pos.m_y > (480.0 - ai_spr.m_height)) // screen height - sprite height
        {
            ai_pos.m_y = (480.0 - ai_spr.m_height);
        }
	}
} // namespace ep
