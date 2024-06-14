///
/// Window.hpp
/// FlecsPong
///
/// Refer to LICENSE.txt for more details.
///

#ifndef FLECSPONG_CORE_WINDOW_HPP_
#define FLECSPONG_CORE_WINDOW_HPP_

#include <cstdint>
#include <string_view>

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

namespace fp {

///
/// Basic window abstraction over SDL2 for this demo app.
///
class Window final
{
public:
    ///
    /// Default constructor.
    ///
    Window();

    ///
    /// Destructor.
    ///
    ~Window();

    ///
    /// Construct a window.
    ///
    /// \param title Title of the window.
    /// \param w Width of the window.
    /// \param h Height of the window.
    /// \param flags SDL2 Window flags.
    ///
    void create(std::string_view title, const int w, const int h,
            std::uint32_t flags);

    ///
    /// Close the window.
    ///
    void close() noexcept;

    ///
    /// Close the window for real.
    ///
    void destroy() noexcept;

    ///
    /// Is the window open?
    ///
    [[nodiscard]] const bool is_open() const noexcept;

    ///
    /// Retrieve the renderer.
    ///
    /// \return CONST pointer to SDL_Renderer object. We want it const because we don't want the pointer to change.
    ///
    [[nodiscard]] SDL_Renderer* renderer() const noexcept;

    ///
    /// SDL2 input event data.
    ///
    SDL_Event m_event;

private:
    ///
    /// Window state.
    ///
    bool m_is_open;

    ///
    /// Pointer to window data.
    ///
    SDL_Window *m_window;

    ///
    /// Pointer to renderer data.
    ///
    SDL_Renderer *m_renderer;
};

} // namespace ep

#endif
