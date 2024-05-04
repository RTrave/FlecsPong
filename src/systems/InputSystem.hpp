///
/// MoveSystem.hpp
/// FlecsPong
///
/// Refer to LICENSE.txt for more details.
///

#ifndef FLECSPONG_SYSTEMS_INPUTSYSTEM_HPP_
#define FLECSPONG_SYSTEMS_INPUTSYSTEM_HPP_

#include <flecs.h>
#include <SDL2/SDL_events.h>

#include "../core/Window.hpp"
#include "../components/All.hpp"

namespace fp
{
///
/// This class will take data from the appropriate components
/// and update the position based on input.
///
class InputSystem final
{
public:
    ///
    /// Default constructor.
    ///
    InputSystem(Window* window);

    ///
    /// Default destructor.
    ///
    ~InputSystem() = default;

    ///
    /// Pointer to window data.
    ///
    Window* m_window;
};

void inputSystem_process(flecs::iter &it, Player *player, Position *pos);

} // namespace ep

#endif
