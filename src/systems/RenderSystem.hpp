///
/// RenderSystem.hpp
/// FlecsPong
///
/// Refer to LICENSE.txt for more details.
///

#ifndef FLECSPONG_SYSTEMS_RENDERSYSTEM_HPP_
#define FLECSPONG_SYSTEMS_RENDERSYSTEM_HPP_

#include "../core/Window.hpp"

namespace fp
{
	///
	/// This class will take data from the appropriate components
	/// and render them to the screen.
	///
	class RenderSystem final
	{
	public:
		///
		/// Default constructor.
		///
		RenderSystem(Window* window);

		///
		/// Default destructor.
		///
		~RenderSystem() = default;

		///
		/// Renders all entities.
		///
		/// \param window The Window to render to.
		/// \param world The registry to retrieve entities from.
		///
		void render(Window& window, flecs::world& world);

        ///
        /// Pointer to window data.
        ///
        Window* m_window;
	};

//	void renderSystem_init(Window* window);
    void renderSystem_flush(flecs::iter &it);
	void renderSystem_process(flecs::iter &it, Sprite* spr, Position* pos);
    void renderSystem_draw(flecs::iter &it);

} // namespace ep

#endif
