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

#include "../core/Window.hpp"
#include "../components/Player.hpp"
#include "../events/KeyDown.hpp"
#include "../events/KeyUp.hpp"

namespace fp
{
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

    ///
    /// Called when a key is pressed.
    ///
    /// \param key_down Key Down Event.
    ///
    void on_key_down(const KeyDown &key_down, flecs::world &world) noexcept;

    ///
    /// Called when a key is released.
    ///
    /// \param key_up Key Up Event.
    ///
    void on_key_up(const KeyUp &key_up, flecs::world &world) noexcept;

    ///
    /// Process events and update entities accordingly.
    ///
    /// \param time DeltaTime or something similar from fixed-timestep gameloop.
    /// \param registry The registry to retrieve entities from.
    ///
    void update(const double time, flecs::world &world);
    flecs::system m_system;
    void init(flecs::world *world);
    flecs::world *m_world;
    void processPlayer(flecs::iter &it, Player *plr, Position *pos);

    ///
    /// Current movement of player.
    ///
    Player::MoveDirection m_player_movement;
};

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

//void inputSystem_init(Window *window);
void inputSystem_process(flecs::iter &it, Player *player, Position *pos);
void moveSystem_processPlayer(flecs::iter &it, Player *plr, Position *pos);
void moveSystem_processBall(flecs::iter &it, Ball *ball, Position *pos);

} // namespace ep

#endif
