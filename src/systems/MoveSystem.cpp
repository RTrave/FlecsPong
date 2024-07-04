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

#include "MoveSystem.hpp"

namespace fp
{

void moveSystem_process(flecs::iter &it, size_t i, Velocity &vel, Position &pos, const Sprite &spr)
{
    const double time = it.delta_time()*1000;
//    printf("time = %f %s %f %f\n", time, it.entity(i).name().c_str(),
//            pos.m_x, vel.m_vel_x);

    // Iterate entities
//    while (it.next()) {
//    for (auto i : it) {

        pos.m_x += vel.m_vel_x * time;
        pos.m_y += vel.m_vel_y * time;

//        printf("time = %f %s %f %f\n", time, it.entity(i).name().c_str(),
//                pos.m_x, vel.m_vel_x);
        // Lock Paddle to screen.
        if (pos.m_y < 0.0)
        {
            if(it.entity(i).has<Paddle>())
            {
                pos.m_y = 0.0;
            }
        }
        else if (pos.m_y > (480.0 - spr.m_height)) // screen width - sprite width
        {
            if(it.entity(i).has<Paddle>())
            {
                pos.m_y = (480.0 - spr.m_height);
            }
        }
//        if (pos[i].m_y < 0.0)
//        {
//            pos[i].m_y = 0.0;
//            if(it.entity(i).has<Ball>())
//                vel[i].m_vel_y = -vel[i].m_vel_y;
//            else
//                vel[i].m_vel_y = 0.0;
//        }
//        else if (pos[i].m_y > (480.0 - spr[i].m_height)) // screen width - sprite width
//        {
//            pos[i].m_y = (480.0 - spr[i].m_height);
//            if(it.entity(i).has<Ball>())
//                vel[i].m_vel_y = -vel[i].m_vel_y;
//            else
//                vel[i].m_vel_y = 0.0;
//        }
//    }
//    }
}

} // namespace ep
