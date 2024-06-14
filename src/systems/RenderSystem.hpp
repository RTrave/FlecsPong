///
/// RenderSystem.hpp
/// FlecsPong
///
/// Refer to LICENSE.txt for more details.
///

#ifndef FLECSPONG_SYSTEMS_RENDERSYSTEM_HPP_
#define FLECSPONG_SYSTEMS_RENDERSYSTEM_HPP_

namespace fp {

class Window;

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
    RenderSystem(Window *window);

    ///
    /// Default destructor.
    ///
    ~RenderSystem() = default;

    ///
    /// Pointer to window data.
    ///
    Window *m_window;
};

//	void renderSystem_init(Window* window);
void renderSystem_flush(flecs::iter &it);
void renderSystem_process(flecs::iter &it, const Sprite *spr,
        const Position *pos);
void renderSystem_draw(flecs::iter &it);

} // namespace ep

#endif
