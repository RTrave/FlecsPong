///
/// Player.hpp
/// FlecsPong
///
/// Refer to LICENSE.txt for more details.
///

#ifndef FLECSPONG_COMPONENTS_PLAYER_HPP_
#define FLECSPONG_COMPONENTS_PLAYER_HPP_

namespace fp
{
	///
	/// Makes an entity a ball.
	///
	class Player final
	{
	public:
		///
		/// Enum representing movement state.
		///
		enum class MoveDirection : short
		{
			STOPPED,
			NORTH,
			SOUTH
		};

		///
		/// Default constructor.
		///
		Player() noexcept;

		///
		/// Move constructor.
		///
		Player(Player&&) noexcept;

		///
		/// Default destructor.
		///
		~Player() noexcept = default;

		///
		/// Move assignment overload.
		///
		Player& operator=(Player&&) noexcept;

	public:
		///
		/// \brief Player movement direction.
		///
		/// False is down, True is up.
		///
		MoveDirection m_movement;
	};
} // namespace ep

#endif