#pragma once

#include <fmt/format.h>
#include <utils/Containers.h>

namespace Logging
{

    template <typename...Args>
    void log( const char* format, Args...args )
    {
        if constexpr(sizeof...(Args) == 0)
        {
            fmt::print( "{}\n", format );
        }

        else
        {
            fmt::print( format, std::forward<Args>(args)... );
        }
    }

    template <typename T>
    void log(T const& t)
    {
        fmt::print( "{}\n", t );
    }

    template <typename T>
    void log(std::vector<T> const& t)
    {
        for (auto const& item : t)
        {
            log(item);
        }
    }

    template <>
    void log( Vector2i const& t );

    template <>
    void log( RectI const& t);

    template <>
    void log( SDL_Rect const& t );
}

