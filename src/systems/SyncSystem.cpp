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

#include "SyncSystem.hpp"

namespace fp
{

void syncSystem_process(flecs::iter &it, const Position *pos, RenderPosition *rpos)
{
//    const double time = it.delta_time();

    // Iterate entities
    for (auto i : it)
    {
        rpos[i].m_x = pos[i].m_x;
        rpos[i].m_y = pos[i].m_y;
    }
}

} // namespace ep
