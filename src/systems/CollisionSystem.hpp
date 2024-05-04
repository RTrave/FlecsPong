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
        flecs::entity player;
        flecs::entity ai;
        flecs::entity ball;
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


	void collisionSystem_process(flecs::iter& it, Ball* ball, Position* ball_pos, Sprite* ball_spr);

} // namespace ep

#endif
