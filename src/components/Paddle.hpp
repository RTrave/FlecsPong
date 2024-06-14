///
/// Player.hpp
/// FlecsPong
///
/// Refer to LICENSE.txt for more details.
///

#ifndef FLECSPONG_COMPONENTS_PADDLE_HPP_
#define FLECSPONG_COMPONENTS_PADDLE_HPP_

namespace fp {

///
/// Makes an entity a ball.
///
class Paddle final
{
public:
    ///
    /// Enum representing movement state.
    ///
    enum class MoveDirection : short
    {
        STOPPED, NORTH, SOUTH
    };

    ///
    /// Default constructor.
    ///
    Paddle() noexcept = default;

    ///
    /// Default destructor.
    ///
    ~Paddle() noexcept = default;

public:
    ///
    /// \brief Player movement direction.
    ///
    MoveDirection m_movement;
    ///
    /// \brief Player movement velocity.
    ///
    /// False is down, True is up.
    ///
    double m_velocity;
};

} // namespace ep

#endif
