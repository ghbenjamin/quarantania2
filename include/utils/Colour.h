#pragma once

#include <SDL2/SDL.h>

class Colour
{
public:

    Colour();
    explicit Colour( SDL_Color const& colour );
    Colour( uint8_t r, uint8_t g, uint8_t b, uint8_t a );

    ~Colour() = default;

    SDL_Color const& raw() const;
    Colour withAlpha(uint8_t val) const;

    bool operator==(const Colour &rhs) const;
    bool operator!=(const Colour &rhs) const;

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

private:
    SDL_Color m_colour;

};