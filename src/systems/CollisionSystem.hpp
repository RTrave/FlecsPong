///
/// CollisionSystem.hpp
/// FlecsPong
///
/// Refer to LICENSE.txt for more details.
///

#ifndef FLECSPONG_SYSTEMS_COLLISIONSYSTEM_HPP_
#define FLECSPONG_SYSTEMS_COLLISIONSYSTEM_HPP_

namespace fp {

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

void collisionSystem_process(flecs::iter &it);

} // namespace ep

#endif
