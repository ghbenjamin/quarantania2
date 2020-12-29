#pragma once

#include <numeric>

Vector2f findCentroid( std::vector<Vector2i> points )
{
    Vector2i sum = {0, 0};

    for (auto const& p : points)
    {
        sum += p;
    }

    float total = (float) points.size();
    Vector2f avg = { sum.x() / total, sum.y() / total };

    return avg;
}