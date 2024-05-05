///
/// Sprite.hpp
/// FlecsPong
///
/// Refer to LICENSE.txt for more details.
///

#ifndef FLECSPONG_COMPONENTS_SPRITE_HPP_
#define FLECSPONG_COMPONENTS_SPRITE_HPP_

#include <SDL2/SDL_pixels.h>

namespace fp
{
	///
	/// A component that describes a sprite or texture.
	/// In this case our 'sprites' are coloured rectangles and circles.
	///
    struct Sprite
	{
		///
		/// Stores how wide the rectangle is (0 means a Circle).
		///
		int m_width;

		///
		/// Stores how high the rectangle is (radius in case of circle).
		///
		int m_height;

		///
		/// Stores the radius of a circle.
		///
		int m_radius;

		///
		/// Stores the colour of the sprite.
		///
		SDL_Colour m_colour;
	};
} // namespace ep

#endif
