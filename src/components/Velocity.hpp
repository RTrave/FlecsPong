///
/// Ball.hpp
/// FlecsPong
///
/// Refer to LICENSE.txt for more details.
///

#ifndef FLECSPONG_COMPONENTS_VELOCITY_HPP_
#define FLECSPONG_COMPONENTS_VELOCITY_HPP_

namespace fp
{
	///
	/// Makes an entity a ball.
	///
    struct Velocity
	{
		///
		/// The X velocity of the ball.
		///
		double m_vel_x;

		///
		/// The Y velocity of the ball.
		///
		double m_vel_y;
	};
} // namespace ep

#endif
