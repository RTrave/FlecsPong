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
		auto ai_query = ecs.query<AI, Position>();
        auto ball_query = ecs.query<Ball, Position>();
		ai_query.each([&](flecs::entity e, AI& ai, Position& ai_pos) {
		    ai.m_x = ai_pos.m_x;
		    ai.m_y = ai_pos.m_y;

		    ball_query.iter([&](flecs::iter& it, Ball* ball, Position* ball_pos) {
		        for (auto i : it) {
		            if (ball_pos[i].m_y > ai_pos.m_y)
                    {
                        ai_pos.m_y += 2.5;
                    }
                    else if (ball_pos[i].m_y < ai_pos.m_y)
                    {
                        ai_pos.m_y -= 2.5;
                    }
		        }
            });
		});

		ai_query.each([&](AI& ai, Position& ai_pos) {
			ai.m_x = ai_pos.m_x;
			ai.m_y = ai_pos.m_y;

			ball_query.each([&](Ball& ball, Position& ball_pos) {
				if (ball_pos.m_y > ai_pos.m_y)
				{
					ai_pos.m_y += 2.5;
				}
				else if (ball_pos.m_y < ai_pos.m_y)
				{
					ai_pos.m_y -= 2.5;
				}
			});
		});
	}

void aiSystem_process(flecs::iter &it, AI *ai, Position *ai_pos)
{
    ai->m_x = ai_pos->m_x;
    ai->m_y = ai_pos->m_y;
//    const auto& ball_pos = it.world().get<Position>("Ball")[0];
//    printf("TEST: %f\n", ball_pos.m_x);

//    auto ball_query = it.world().query<Ball, Position>();
    it.world().each([&](flecs::entity e, Ball& ball, Position& ball_pos) {
//        for (auto i : it) {
            if (ball_pos.m_y > ai_pos->m_y)
            {
                ai_pos->m_y += 2.5;
            }
            else if (ball_pos.m_y < ai_pos->m_y)
            {
                ai_pos->m_y -= 2.5;
            }
//        }
    });
}

} // namespace ep