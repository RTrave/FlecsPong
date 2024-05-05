///
/// CollisionSystem.hpp
/// FlecsPong
///
/// Refer to LICENSE.txt for more details.
///

#ifndef FLECSPONG_SYSTEMS_COLLISIONSYSTEM_HPP_
#define FLECSPONG_SYSTEMS_COLLISIONSYSTEM_HPP_

namespace fp
{
///
/// Holds information about possible colliders.
///
struct CollisionHolder
{
    flecs::entity player1;
    flecs::entity player2;
    int score_player1;
    int score_player2;
};

///
/// This system will ensure the ai is always following the ball, by centering the ai paddle
/// onto the ball.
///
class CollisionSystem final
{
public:
    ///
    /// Default constructor.
    ///
    CollisionSystem() = default;

    ///
    /// Default destructor.
    ///
    ~CollisionSystem() = default;
};

void collisionSystem_process(flecs::iter &it, Ball *ball, Position *pos,
                             Velocity *vel, Sprite *spr);

} // namespace ep

#endif
