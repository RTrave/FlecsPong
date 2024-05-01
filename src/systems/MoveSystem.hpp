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
		void on_key_down(const KeyDown& key_down) noexcept;

		///
		/// Called when a key is released.
		///
		/// \param key_up Key Up Event.
		///
		void on_key_up(const KeyUp& key_up) noexcept;

		///
		/// Process events and update entities accordingly.
		///
		/// \param time DeltaTime or something similar from fixed-timestep gameloop.
		/// \param registry The registry to retrieve entities from.
		///
		void update(const double time, flecs::world& world);

		///
		/// Current movement of player.
		///
		Player::MoveDirection m_player_movement;
	};
} // namespace ep

#endif
