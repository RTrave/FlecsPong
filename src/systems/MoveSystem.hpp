///
/// MoveSystem.hpp
/// FlecsPong
///
/// Refer to LICENSE.txt for more details.
///

#ifndef FLECSPONG_SYSTEMS_MOVESYSTEM_HPP_
#define FLECSPONG_SYSTEMS_MOVESYSTEM_HPP_

#include <flecs.h>
#include <SDL2/SDL_events.h>

#include "../components/All.hpp"

namespace fp {

///
/// This class will take data from the appropriate components
/// and update the position based on input.
///
class MoveSystem final
{
public:
    ///
    /// Default constructor.
    ///
    MoveSystem() = default;

    ///
    /// Default destructor.
    ///
    ~MoveSystem() = default;
};

void moveSystem_process(flecs::iter &it, size_t i, Velocity &vel, Position &pos,
        const Sprite &spr);

} // namespace ep

#endif
