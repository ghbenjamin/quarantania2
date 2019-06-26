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

    void x( T val )
    { m_x = val; }

    const T y() const
    { return m_y; }

    void y( T val )
    { m_y = val; }

    Vector2 operator+(Vector2 const &other) const
    {
        return Vector2(x() + other.x(), y() + other.y());
    }

    Vector2 operator-(Vector2 const &other) const
    {
        return Vector2(x() - other.x(), y() - other.y());
    }

    Vector2& operator+=(const Vector2& rhs){

        this->m_x += rhs.m_x;
        this->m_y += rhs.m_y;
        return *this;
    }

    Vector2& operator-=(const Vector2& rhs){

        this->m_x -= rhs.m_x;
        this->m_y -= rhs.m_y;
        return *this;
    }

    Vector2 operator*(int other) const
    {
        return Vector2(x() * other, y() * other);
    }

    Vector2 operator*(float other) const
    {
        return Vector2((float)(x() * other), (float)(y() * other));
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

    template <std::size_t N>
    decltype(auto) get() const
    {
        if      constexpr (N == 0) return m_x;
        else if constexpr (N == 1) return m_y;
    }

private:

    T m_x;
    T m_y;

};

// STD Magic for structured bindings
namespace std {
    template <typename T>
    struct tuple_size<Vector2<T>>
            : std::integral_constant<std::size_t, 2> {};

    template<typename T, std::size_t N>
    struct tuple_element<N, Vector2<T>> {
        using type = decltype(std::declval<T>());
    };
}

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

    void x(T val)
    {
        m_x = val;
        m_rect.x = (int) m_x;
    }

    void y(T val)
    {
        m_y = val;
        m_rect.y = (int) m_y;
    }

    void w(T val)
    {
        m_w = val;
        m_rect.w = (int) m_w;
    }

    void h(T val)
    {
        m_h = val;
        m_rect.h = (int) m_h;
    }

    Vector2<T> left() const
    {
        return { m_x, m_y };
    }

    Vector2<T> right() const
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

    SDL_Rect toSDL( ) const
    {
        return SDL_Rect{m_x, m_y, m_w, m_h};
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

    template <std::size_t N>
    decltype(auto) get() const
    {
        if      constexpr (N == 0) return m_x;
        else if constexpr (N == 1) return m_y;
        else if constexpr (N == 2) return m_w;
        else if constexpr (N == 3) return m_h;
    }

private:

    T m_x;
    T m_y;
    T m_w;
    T m_h;

    SDL_Rect m_rect;
};

// STD Magic for structured bindings
namespace std {
    template <typename T>
    struct tuple_size<Rect<T>>
            : std::integral_constant<std::size_t, 4> {};

    template<typename T, std::size_t N>
    struct tuple_element<N, Rect<T>> {
        using type = decltype(std::declval<T>());
    };
}


typedef Vector2<int> Vector2i;
typedef Vector2<float> Vector2f;
typedef Rect<int> RectI;
typedef Rect<float> RectF;