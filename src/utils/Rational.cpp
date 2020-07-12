#include <utils/Rational.h>
#include <numeric>

Rational::Rational()
    : m_top(1), m_bottom(1)
{
}

Rational::Rational(int t, int b)
    : m_top(t), m_bottom(b)
{
}

Rational Rational::operator+(Rational const &other)
{
    m_top += other.m_top;
    m_bottom *= other.m_bottom;
    reduce();

    return *this;
}

void Rational::reduce()
{
    auto gcd = std::gcd( m_top, m_bottom );
    m_top /= gcd;
    m_bottom /= gcd;
}

int Rational::getTop() const
{
    return m_top;
}

void Rational::setTop(int top)
{
    m_top = top;
}

int Rational::getBottom() const
{
    return m_bottom;
}

void Rational::setBottom(int bottom)
{
    m_bottom = bottom;
}

