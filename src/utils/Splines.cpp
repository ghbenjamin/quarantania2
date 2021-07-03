#include <utils/Splines.h>
#include <utils/Assert.h>

SplineSeg::SplineSeg(double x3, double x2, double x1, double c, double width)
        : x3(x3), x2(x2), x1(x1), c(c), width(width)
{}

double SplineSeg::value(double t) const
{
    return (x3 * t * t * t) + (x2 * t * t) + (x1 * t) + c;
}


void Spline::addSegment(const SplineSeg &seg)
{
    m_segments.push_back( seg );
}

void Spline::addSegment(double x3, double x2, double x1, double c, double width)
{
    m_segments.emplace_back( x3, x2, x1, c, width );
}

double Spline::getValue(double t) const
{
    double curr = t;
    for (auto it = m_segments.begin(); it != m_segments.end(); it++)
    {
        if (curr <= it->width)
        {
            return it->value(t);
        }
        else
        {
            curr -= it->width;
        }
    }

    // The value is out of bounds
    AssertAlwaysMsg( "Spline parameter out of bounds" );
    return 0;
}

