#pragma once

#include <SDL2/SDL.h>
#include <string>
#include <array>

class Colour
{
public:

    Colour();
    Colour( SDL_Color const& colour );
    Colour( uint8_t r, uint8_t g, uint8_t b, uint8_t a );
    Colour( std::string const& str );

    ~Colour() = default;

    SDL_Color const& asSDL() const;
    std::array<float, 4> asOpenGL() const;
    
    Colour withAlpha(uint8_t val) const;
    Colour withAlphaF(float val) const;

    bool operator==(const Colour &rhs) const;
    bool operator!=(const Colour &rhs) const;
    bool operator<(const Colour &rhs) const;
    Colour operator-(Colour const &other) const;

    std::string toString();

    static const Colour Red;
    static const Colour Green;
    static const Colour Yellow;
    static const Colour Blue;
    static const Colour Orange;
    static const Colour Purple;
    static const Colour Cyan;
    static const Colour Magenta;
    static const Colour Lime;
    static const Colour Pink;
    static const Colour Teal;
    static const Colour Lavender;
    static const Colour Brown;
    static const Colour Beige;
    static const Colour Maroon;
    static const Colour Mint;
    static const Colour Olive;
    static const Colour Apricot;
    static const Colour Navy;
    static const Colour Grey;
    static const Colour White;
    static const Colour Black;

    static const Colour fromName( std::string const& name );

private:
    
    static float intToFloat( std::uint8_t val );
    static std::uint8_t floatToInt( float val );
    
    SDL_Color m_colour;
    float m_r, m_g, m_b, m_a;
};