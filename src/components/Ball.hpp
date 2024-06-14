///
/// Ball.hpp
/// FlecsPong
///
/// Refer to LICENSE.txt for more details.
///

#ifndef FLECSPONG_COMPONENTS_BALL_HPP_
#define FLECSPONG_COMPONENTS_BALL_HPP_

namespace fp {

///
/// Makes an entity a ball.
///
struct Ball
{
    ///
    /// The number of ticks the ball is bounce immune for
    ///
    int m_bounce_immune_ticks;

    ///
    /// The original X velocity of the ball.
    ///
    double m_initial_vel_x;

    ///
    /// The original Y velocity of the ball.
    ///
    double m_initial_vel_y;
};

} // namespace ep

#endif
