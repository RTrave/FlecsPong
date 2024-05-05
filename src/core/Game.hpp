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
//#include "../systems/InputSystem.hpp"
#include "../systems/AISystem.hpp"
#include "../systems/CollisionSystem.hpp"
#include "../systems/MoveSystem.hpp"
#include "../systems/RenderSystem.hpp"

#include "Window.hpp"

namespace fp
{

class InputSystem;

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

		flecs::entity createBall();

        void switchPVP();

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
		/// The default flecs registry to hold and manage our entities.
		///
		flecs::world m_ecs;

		///
		/// The AI system.
		///
		AISystem *m_ai_system;

		///
		/// The collision system.
		///
		CollisionSystem m_collision_system;

		///
		/// Holds collideables for collision system.
		///
		CollisionHolder m_collideables;

		///
		/// The movement system.
		///
		MoveSystem m_move_system;

        ///
        /// The movement system.
        ///
        InputSystem *m_input_system;

		///
		/// The Render system.
		///
		RenderSystem *m_render_system;
	};
} // namespace ep

#endif
