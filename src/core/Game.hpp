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
#include <SDL2/SDL.h>

#include "../components/All.hpp"
#include "../systems/CollisionSystem.hpp"
#include "Window.hpp"

namespace fp
{

class InputSystem;
class AISystem;
class SyncSystem;
class RenderSystem;

#define THREAD_STATE_OFF static_cast<int>(0)
#define THREAD_STATE_STARTING static_cast<int>(1)
#define THREAD_STATE_ON static_cast<int>(2)
#define THREAD_STATE_STOPING static_cast<int>(3)

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

    void threadLoop();

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
    /// The movement system.
    ///
    SyncSystem *m_sync_system;

    ///
    /// The Render system.
    ///
    RenderSystem *m_render_system;

    flecs::system m_inputsys;
    flecs::system m_aisys;
    flecs::system m_movesys;
    flecs::system m_collisionsys;
    flecs::system m_syncsys;
    flecs::system m_rendersys_flush;
    flecs::system m_rendersys;
    flecs::system m_rendersys_draw;

    SDL_Thread *m_systemThread;
    int m_systemFlag;
    SDL_mutex *m_systemMutex;
    Uint32 m_threadid;
    double m_frame_time;
    bool m_drawnFlag;

    ///
    /// Balls to create
    ///
    int m_ballsToCreate = 0;
};
} // namespace ep

#endif
