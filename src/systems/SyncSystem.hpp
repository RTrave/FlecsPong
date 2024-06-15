///
/// MoveSystem.hpp
/// FlecsPong
///
/// Refer to LICENSE.txt for more details.
///

#ifndef FLECSPONG_SYSTEMS_SYNCSYSTEM_HPP_
#define FLECSPONG_SYSTEMS_SYNCSYSTEM_HPP_

#include <flecs.h>
#include <SDL2/SDL_events.h>

#include "../components/All.hpp"

namespace fp
{
///
/// This class will take data from the appropriate components
/// and update the position based on input.
///
class SyncSystem final
{
public:
    ///
    /// Default constructor.
    ///
    SyncSystem() = default;

    ///
    /// Default destructor.
    ///
    ~SyncSystem() = default;
};

void syncSystem_process(flecs::iter &it, const Position *pos, RenderPosition *rpos);

} // namespace ep

#endif
