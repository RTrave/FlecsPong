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

        flecs::world* world;
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

		///
		/// Process events and update entities accordingly.
		///
		/// \param time DeltaTime or something similar from fixed-timestep gameloop.
		/// \param holder Used instead of something like a Dynamic Tree which is out of scope for this.
		///
		void update(const double time, CollisionHolder& holder);
	};
} // namespace ep

#endif
