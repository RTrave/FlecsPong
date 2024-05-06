///
/// Window.cpp
/// FlecsPong
///
/// Refer to LICENSE.txt for more details.
///

#include "../Log.hpp"
#include "Window.hpp"

namespace fp
{
	Window::Window()
	    : m_is_open {true}, m_window {nullptr}, m_renderer {nullptr}
	{
	}

	Window::~Window()
	{
	}

	void Window::create(std::string_view title, const int w, const int h, std::uint32_t flags)
	{
		const auto str_title = static_cast<std::string>(title);

		m_window = SDL_CreateWindow(str_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, flags);
		if (!m_window)
		{
			throw_sdl2_exception("Window failed to be created.");
		}

		m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED); //  | SDL_RENDERER_PRESENTVSYNC
		if (!m_renderer)
		{
			throw_sdl2_exception("Renderer failed to be created.");
		}
	}

	void Window::close() noexcept
	{
		m_is_open = false;
	}

    void Window::destroy() noexcept
    {
        SDL_DestroyRenderer(m_renderer);
        SDL_DestroyWindow(m_window);
    }

	const bool Window::is_open() const noexcept
	{
		return m_is_open;
	}

	SDL_Renderer* Window::renderer() const noexcept
	{
		return m_renderer;
	}
} // namespace ep
