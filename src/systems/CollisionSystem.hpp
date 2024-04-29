///
/// CollisionSystem.hpp
/// EnttPong
///
/// Refer to LICENSE.txt for more details.
///

#ifndef ENTTPONG_SYSTEMS_COLLISIONSYSTEM_HPP_
#define ENTTPONG_SYSTEMS_COLLISIONSYSTEM_HPP_

//#include <entt/fwd.hpp>

namespace ep
{
	///
	/// Holds information about possible colliders.
	///
	struct CollisionHolder
	{
//		entt::entity player;
//		entt::entity ai;
//		entt::entity ball;
        flecs::entity player;
        flecs::entity ai;
        flecs::entity ball;

//		entt::registry* registery;
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
