#include <utils/Colour.h>
#include <utils/Assert.h>
#include <fmt/format.h>

const Colour Colour::Red = SDL_Colour{ 230, 25, 75, 255 };
const Colour Colour::Green = SDL_Colour{ 60, 180, 75, 255 };
const Colour Colour::Yellow = SDL_Colour{ 255, 225, 25, 255 };
const Colour Colour::Blue = SDL_Colour{ 0, 130, 200, 255 };
const Colour Colour::Orange = SDL_Colour{ 245, 130, 48, 255 };
const Colour Colour::Purple = SDL_Colour{ 145, 30, 180, 255 };
const Colour Colour::Cyan = SDL_Colour{ 70, 240, 240, 255 };
const Colour Colour::Magenta = SDL_Colour{ 240, 50, 230, 255 };
const Colour Colour::Lime = SDL_Colour{ 210, 245, 60, 255 };
const Colour Colour::Pink = SDL_Colour{ 250, 190, 190, 255 };
const Colour Colour::Teal = SDL_Colour{ 0, 128, 128, 255 };
const Colour Colour::Lavender = SDL_Colour{ 230, 190, 255, 255 };
const Colour Colour::Brown = SDL_Colour{ 170, 110, 40, 255 };
const Colour Colour::Beige = SDL_Colour{ 255, 250, 200, 255 };
const Colour Colour::Maroon = SDL_Colour{ 128, 0, 0, 255 };
const Colour Colour::Mint = SDL_Colour{ 170, 255, 195, 255 };
const Colour Colour::Olive = SDL_Colour{ 128, 128, 0, 255 };
const Colour Colour::Apricot = SDL_Colour{ 255, 215, 180, 255 };
const Colour Colour::Navy = SDL_Colour{ 0, 0, 128, 255 };
const Colour Colour::Grey = SDL_Colour{ 128, 128, 128, 255 };
const Colour Colour::White = SDL_Colour{ 255, 255, 255, 255 };
const Colour Colour::Black = SDL_Colour{ 0, 0, 0, 255 };

Colour::Colour(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
        : m_colour{r, g, b, a}, m_r(intToFloat(r)), m_g(intToFloat(g)), m_b(intToFloat(b)), m_a(intToFloat(a)) {}

Colour::Colour(const std::string& str)
{
    if ( str.size() == 7 && str.at(0) == '#' )
    {
        unsigned short r, g, b;
        sscanf_s( str.substr(1, 6).c_str(), "%2hx%2hx%2hx", &r, &g, &b );

        m_colour = { (std::uint8_t)r, (std::uint8_t)g, (std::uint8_t)b, 255 };
        m_r = intToFloat(r);
        m_g = intToFloat(g);
        m_b = intToFloat(b);
        m_a = 1.0f;
    }
    else
    {
        AssertAlwaysMsg( fmt::format( "Unknown colour string: {}", str ) );
    }
}


SDL_Color const &Colour::asSDL() const
{ return m_colour; }

Colour::Colour() : m_colour{0, 0, 0, 0}, m_r(0), m_g(0), m_b(0), m_a(0) {}

Colour::Colour(SDL_Color const &colour) : m_colour(colour),
    m_r(intToFloat(colour.r)), m_g(intToFloat(colour.g)),
    m_b(intToFloat(colour.b)), m_a(intToFloat(colour.a))  {}

bool Colour::operator==(const Colour &rhs) const
{
    return m_colour.r == rhs.m_colour.r
           && m_colour.g == rhs.m_colour.g
           && m_colour.b == rhs.m_colour.b
           && m_colour.a == rhs.m_colour.a;
}

bool Colour::operator!=(const Colour &rhs) const
{
    return !(rhs == *this);
}

Colour Colour::withAlpha(uint8_t val) const
{
    Colour c = *this;
    c.m_colour.a = val;
    c.m_a = intToFloat(val);
    return c;
}

Colour Colour::withAlphaF(float val) const
{
    return withAlpha( (uint8_t)(val * 255.f) );
}


bool Colour::operator<(const Colour &rhs) const
{
    if (m_colour.r < rhs.m_colour.r)
        return true;
    if (rhs.m_colour.r < m_colour.r)
        return false;
    if (m_colour.g < rhs.m_colour.g)
        return true;
    if (rhs.m_colour.g < m_colour.g)
        return false;
    if (m_colour.b < rhs.m_colour.b)
        return true;
    if (rhs.m_colour.b < m_colour.b)
        return false;
    return m_colour.a < rhs.m_colour.a;
}

std::array<float, 4> Colour::asOpenGL() const
{
   return { m_r, m_g, m_b, m_a };
}


Colour Colour::operator-( Colour const &other ) const
{
    return Colour(
        m_colour.r - other.m_colour.r,
        m_colour.g - other.m_colour.g,
        m_colour.b - other.m_colour.b,
        m_colour.a - other.m_colour.a
    );
}

float Colour::intToFloat( std::uint8_t val )
{
    return (float) val / 255.0f;
}

std::uint8_t Colour::floatToInt( float val )
{
    return (std::uint8_t)(val * 255.0f);
}

const Colour Colour::fromName(const std::string &name)
{
    if (name == "Red")
    {
        return Colour::Red;
    }
    else if (name == "Green")
    {
        return Colour::Green;
    }
    else if (name == "Yellow")
    {
        return Colour::Yellow;
    }
    else if (name == "Blue")
    {
        return Colour::Blue;
    }
    else if (name == "Orange")
    {
        return Colour::Orange;
    }
    else if (name == "Purple")
    {
        return Colour::Purple;
    }
    else if (name == "Cyan")
    {
        return Colour::Cyan;
    }
    else if (name == "Magenta")
    {
        return Colour::Magenta;
    }
    else if (name == "Lime")
    {
        return Colour::Lime;
    }
    else if (name == "Pink")
    {
        return Colour::Pink;
    }
    else if (name == "Teal")
    {
        return Colour::Teal;
    }
    else if (name == "Lavender")
    {
        return Colour::Lavender;
    }
    else if (name == "Brown")
    {
        return Colour::Brown;
    }
    else if (name == "Beige")
    {
        return Colour::Beige;
    }
    else if (name == "Maroon")
    {
        return Colour::Maroon;
    }
    else if (name == "Mint")
    {
        return Colour::Mint;
    }
    else if (name == "Olive")
    {
        return Colour::Olive;
    }
    else if (name == "Apricot")
    {
        return Colour::Apricot;
    }
    else if (name == "Navy")
    {
        return Colour::Navy;
    }
    else if (name == "Grey")
    {
        return Colour::Grey;
    }
    else if (name == "White")
    {
        return Colour::White;
    }
    else
    {
        return Colour::Black;
    }
}

std::string Colour::toString()
{
    return fmt::format( "[{}, {}, {}, {}]", m_r, m_g, m_b, m_a );
}

