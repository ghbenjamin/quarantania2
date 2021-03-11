#include <utils/Colour.h>
#include <utils/Assert.h>

const Colour Colour::Red = { 230, 25, 75, 255 };
const Colour Colour::Green = { 60, 180, 75, 255 };
const Colour Colour::Yellow = { 255, 225, 25, 255 };
const Colour Colour::Blue = { 0, 130, 200, 255 };
const Colour Colour::Orange = { 245, 130, 48, 255 };
const Colour Colour::Purple = { 145, 30, 180, 255 };
const Colour Colour::Cyan = { 70, 240, 240, 255 };
const Colour Colour::Magenta = { 240, 50, 230, 255 };
const Colour Colour::Lime = { 210, 245, 60, 255 };
const Colour Colour::Pink = { 250, 190, 190, 255 };
const Colour Colour::Teal = { 0, 128, 128, 255 };
const Colour Colour::Lavender = { 230, 190, 255, 255 };
const Colour Colour::Brown = { 170, 110, 40, 255 };
const Colour Colour::Beige = { 255, 250, 200, 255 };
const Colour Colour::Maroon = { 128, 0, 0, 255 };
const Colour Colour::Mint = { 170, 255, 195, 255 };
const Colour Colour::Olive = { 128, 128, 0, 255 };
const Colour Colour::Apricot = { 255, 215, 180, 255 };
const Colour Colour::Navy = { 0, 0, 128, 255 };
const Colour Colour::Grey = { 128, 128, 128, 255 };
const Colour Colour::White = { 255, 255, 255, 255 };
const Colour Colour::Black = { 0, 0, 0, 255 };

Colour::Colour(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
        : m_colour{r, g, b, a} {}

Colour::Colour(uint8_t r, uint8_t g, uint8_t b)
    : m_colour{r, g, b, 255} {}

Colour::Colour(const std::string& str)
{
    if ( str.size() == 7 && str.at(0) == '#' )
    {
        unsigned short r, g, b;
        sscanf_s( str.substr(1, 6).c_str(), "%2hx%2hx%2hx", &r, &g, &b );

        m_colour = { (std::uint8_t)r, (std::uint8_t)g, (std::uint8_t)b, 255 };
    }
    else
    {
        AssertAlways();
    }
}


SDL_Color const &Colour::raw() const
{ return m_colour; }

Colour::Colour() : m_colour{0, 0, 0, 0} {}

Colour::Colour(SDL_Color const &colour) : m_colour(colour) {}

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
    return c;
}

uint8_t Colour::getR() const
{
    return m_colour.r;
}

uint8_t Colour::getG() const
{
    return m_colour.g;
}

uint8_t Colour::getB() const
{
    return m_colour.b;
}

uint8_t Colour::getA() const
{
    return m_colour.a;
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

std::array<float, 4> Colour::toOpenGL() const
{
    return {
        m_colour.r / 255.0f,
        m_colour.g / 255.0f,
        m_colour.b / 255.0f,
        m_colour.a / 255.0f,
    };
}

void Colour::setR( std::uint8_t val )
{
    m_colour.r = val;
}

void Colour::setG( std::uint8_t val )
{
    m_colour.g = val;
}

void Colour::setB( std::uint8_t val )
{
    m_colour.b = val;
}

void Colour::setA( std::uint8_t val )
{
    m_colour.a = val;
}