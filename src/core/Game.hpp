///
/// Game.hpp
/// FlecsPong
///
/// MIT LICENSE.
/// Refer to LICENSE.txt for more details.
///

#ifndef FLECSPONG_CORE_GAME_HPP_
#define FLECSPONG_CORE_GAME_HPP_

#include <flecs.h>

#include "../components/All.hpp"
#include "../systems/CollisionSystem.hpp"
#include "Window.hpp"

namespace fp
{

class InputSystem;
class AISystem;
class RenderSystem;

///
/// This class will manage our game data and game loop.
/// Along with the application instance.
///
class Game final
{
public:
    ///
    /// Main constructor.
    ///
    /// \param title Title of the window.
    /// \param w Width of the window.
    /// \param h Height of the window.
    /// \param flags SDL2 Window flags.
    ///
    Game(std::string_view title, const int w, const int h, std::uint32_t flags);

    ///
    /// Destructor.
    ///
    ~Game() noexcept;

    ///
    /// Run the game loop.
    ///
    /// \return Returns game exit code.
    ///
    [[maybe_unused]] const int run();

    ///
    /// Display scores, destroy Balls and reset scores
    ///
    void reset();

    ///
    /// Create a new default Ball
    ///
    flecs::entity createBall();

    ///
    /// Create nb default Balls
    ///
    void createKBalls(int nb);

    ///
    /// Switch Paddle2 from AI to Player and vice-versa
    ///
    void switchPVP();

    ///
    /// The default flecs world to hold and manage our entities.
    /// Public to simplify direct access by Systems
    ///
    flecs::world m_ecs;

private:
    ///
    /// Deleted default constructor.
    ///
    Game() = delete;

    ///
    /// Our main window.
    ///
    Window m_window;

    ///
    /// The AI system.
    ///
    AISystem *m_ai_system;

    ///
    /// Holds collideables for collision system.
    ///
    CollisionHolder m_collideables;

    ///
    /// The movement system.
    ///
    InputSystem *m_input_system;

    ///
    /// The Render system.
    ///
    RenderSystem *m_render_system;

    ///
    /// Balls to create
    ///
    int m_ballsToCreate = 0;
};
} // namespace ep

#endif
