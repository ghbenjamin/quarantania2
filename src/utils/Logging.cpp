#include <utils/Logging.h>

template <>
void Logging::log( Vector2i const& t )
{
    fmt::print( "({}, {})\n", t.x(), t.y() );
}

template<>
void Logging::log(RectI const &t)
{
    fmt::print( "({}, {}, {}, {})\n", t.x(), t.y(), t.w(), t.h() );
}

template<>
void Logging::log(SDL_Rect const &t)
{
    fmt::print( "({}, {}, {}, {})\n", t.x, t.y, t.w, t.h );
}
