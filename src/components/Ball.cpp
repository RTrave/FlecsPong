///
/// Ball.cpp
/// FlecsPong
///
/// Refer to LICENSE.txt for more details.
///

#include "../Random.hpp"

#include "Ball.hpp"

namespace fp
{
	const double Ball::initial_x_vel() const noexcept
	{
		return m_initial_vel_x;
	}

	const double Ball::initial_y_vel() const noexcept
	{
		return m_initial_vel_y;
	}
} // namespace ep
