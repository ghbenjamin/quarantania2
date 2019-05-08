#pragma once

#include <SDL.h>

#include <string>
#include <sstream>
#include <cmath>
#include <vector>
#include <unordered_set>
#include <unordered_map>

template<typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
class Vector2
{
public:

    constexpr Vector2() noexcept
    : m_x(0), m_y(0)
    {}

    constexpr Vector2(T x, T y) noexcept
    : m_x(x), m_y(y)
    {}

    const T x() const
    { return m_x; }

    const T y() const
    { return m_y; }

    Vector2 operator+(Vector2 const &other) const
    {
        return Vector2(x() + other.x(), y() + other.y());
    }

    Vector2 operator*(int other) const
    {
        return Vector2(x() * other, y() * other);
    }

    bool operator==(const Vector2 &rhs) const
    {
        return m_x == rhs.m_x &&
               m_y == rhs.m_y;
    }

    bool operator!=(const Vector2 &rhs) const
    {
        return !(rhs == *this);
    }

    void normalize()
    {
        double magnitude = sqrt(m_x * m_x + m_y * m_y);
        m_x /= magnitude;
        m_y /= magnitude;
    }

    std::string to_string() const
    {
        std::stringstream ss;
        ss << "(" << m_x << ", " << m_y << ")";
        return ss.str();
    }

    template <typename K>
    Vector2<K> convert() const
    {
        return { static_cast<K>(m_x), static_cast<K>(m_y) };
    }

    static const Vector2<T> Null()
    {
        return {0, 0};
    }

    static const Vector2<T> Unit( int i )
    {
        return {i, i};
    }

private:

    T m_x;
    T m_y;

};


template <typename T>
struct Vector2Hash
{
    std::size_t operator()(const Vector2<T>& k) const
    {
        return std::hash<T>()(k.x()) ^ ( std::hash<T>()(k.y()) << 1 );
    }
};


template<typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
class Rect
{
public:
    Rect()
        : m_rect{0, 0, 0, 0},
          m_x(0), m_y(0), m_w(0), m_h(0)
    {}

    Rect(Vector2<T> pos, Vector2<T> size)
        : m_rect{(int) pos.x(), (int) pos.y(), (int) size.x(), (int) size.y()},
          m_x(pos.x()), m_y(pos.y()), m_w(size.x()), m_h(size.y())
    {}

    Rect(T x, T y, T w, T h)
        : m_rect{(int) x, (int) y, (int) w, (int) h},
          m_x((int) x), m_y((int) y), m_w((int) w), m_h((int) h)
    {}

    Rect(SDL_Rect rect)
        : m_rect(rect),
          m_x(rect.x), m_y(rect.y), m_w(rect.w), m_h(rect.h)
    {}


    T x() const
    { return m_x; }

    T y() const
    { return m_y; }

    T w() const
    { return m_w; }

    T h() const
    { return m_h; }

    void setX(T x)
    {
        m_x = x;
        m_rect.x = (int) m_x;
    }

    void setY(T y)
    {
        m_y = y;
        m_rect.y = (int) m_y;
    }

    void setW(T w)
    {
        m_w = w;
        m_rect.w = (int) m_w;
    }

    void setH(T h)
    {
        m_h = h;
        m_rect.h = (int) m_h;
    }

    Vector2<T> left()
    {
        return { m_x, m_y };
    }

    Vector2<T> right()
    {
        return { m_w, m_h };
    }

    void setLeft( Vector2<T> const& left )
    {
        m_x = left.x();
        m_y = left.y();
    }

    void setRight( Vector2<T> const& right )
    {
        m_w = right.x();
        m_h = right.y();
    }

    void translate(Vector2<T> vector)
    {
        m_x += vector.x();
        m_y += vector.y();

        m_rect.x = (int) m_x;
        m_rect.y = (int) m_y;
    }

    bool intersect( Rect<T>const& other )
    {
        return SDL_HasIntersection( &m_rect, &other.raw() ) == SDL_TRUE;
    }

    bool contains( Vector2<T> const& point )
    {
        SDL_Point p = { point.x(), point.y() };
        return SDL_PointInRect( &p, &m_rect ) == SDL_TRUE;
    }

    const SDL_Rect &raw() const
    {
        return m_rect;
    }

    std::string toString() const
    {
        std::stringstream ss;
        ss << "{" << m_x << ", " << m_y << ", " << m_w << ", " << m_h << "}";
        return ss.str();
    }

    bool isEmpty() const
    {
        return m_x == 0 && m_y == 0 && m_w == 0 && m_h == 0;
    }

    static const Rect<T> Null()
    {
        return {0, 0, 0, 0};
    }

    bool operator==(const Rect &rhs) const
    {
        return m_x == rhs.m_x &&
               m_y == rhs.m_y &&
               m_w == rhs.m_w &&
               m_h == rhs.m_h;
    }

    bool operator!=(const Rect &rhs) const
    {
        return !(rhs == *this);
    }

private:

    T m_x;
    T m_y;
    T m_w;
    T m_h;

    SDL_Rect m_rect;
};


typedef Vector2<int> Vector2i;
typedef Vector2<float> Vector2f;
typedef Rect<int> RectI;
typedef Rect<float> RectF;