#pragma once

#include <type_traits>
#include <utils/Containers.h>

// Quick and dirty matrix implementation

template<typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
class Matrix2
{
public:

    constexpr Matrix2() : m_xx(0), m_xy(0), m_yx(0), m_yy(0) {}
    constexpr Matrix2(T xx, T xy, T yx, T yy)
    : m_xx(xx), m_xy(xy), m_yx(yx), m_yy(yy) {}

    ~Matrix2() = default;

    T getXX() const { return m_xx; };
    T getXY() const { return m_xy; };
    T getYX() const { return m_yx; };
    T getYY() const { return m_yy; };

    Vector2<T> transform( Vector2<T> other ) const
    {
        return {
            other.x() * m_xx + other.y() * m_xy,
            other.x() * m_yx + other.y() * m_yy
        };
    }

private:
    T m_xx;
    T m_xy;
    T m_yx;
    T m_yy;
};

typedef Matrix2<int> Matrix2i;
typedef Matrix2<float> Matrix2f;