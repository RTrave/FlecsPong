///
/// AISystem.hpp
/// FlecsPong
///
/// Refer to LICENSE.txt for more details.
///

#ifndef FLECSPONG_SYSTEMS_AISYSTEM_HPP_
#define FLECSPONG_SYSTEMS_AISYSTEM_HPP_

namespace fp
{
	///
	/// This system will ensure the ai is always following the ball, by centering the ai paddle
	/// onto the ball.
	///
	class AISystem final
	{
	public:
		///
		/// Default constructor.
		///
		AISystem() = default;

		///
		/// Default destructor.
		///
		~AISystem() = default;

		///
		/// Process events and update entities accordingly.
		///
		/// \param time Update loop delta time.
		/// \param registry The registry to retrieve entities from.
		///
		void update(const double time, flecs::world& ecs);
	};
void aiSystem_process(flecs::iter& it, AI* ai, Position* ai_pos);
} // namespace ep

#endif
