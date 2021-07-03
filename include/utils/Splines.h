#pragma once

#include <vector>

struct SplineSeg
{
    SplineSeg(double x3, double x2, double x1, double c, double width);

    double x3;
    double x2;
    double x1;
    double c;
    double width;

    double value( double t ) const;
};

class Spline
{
public:

    Spline() = default;
    ~Spline() = default;

    void addSegment( SplineSeg const& seg );
    void addSegment( double x3, double x2, double x1, double c, double width );

    double getValue( double t ) const;

private:

    std::vector<SplineSeg> m_segments;

};