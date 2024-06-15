///
/// RenderPosition.hpp
/// FlecsPong
///
/// Refer to LICENSE.txt for more details.
///

#ifndef FLECSPONG_COMPONENTS_RENDERPOSITION_HPP_
#define FLECSPONG_COMPONENTS_RENDERPOSITION_HPP_

namespace fp {
///
/// A component that describes the position of something on the screen.
///
struct RenderPosition
{
    ///
    /// x pos on screen.
    ///
    double m_x;

    ///
    /// y pos on screen.
    ///
    double m_y;
};
} // namespace ep

#endif
