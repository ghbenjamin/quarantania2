#include <utils/Math.h>
#include <utils/Logging.h>

Vector2f Math::findCentroid( std::vector<Vector2i> points )
{
    Vector2i sum = { 0, 0 };
    
    for ( auto const &p : points )
    {
        sum += p;
    }
    
    float total = (float) points.size();
    Vector2f avg = { sum.x() / total, sum.y() / total };
    
    return avg;
}

std::array<Vector2f, 4> Math::getQuadForLine( Vector2f lineBegin, Vector2f lineEnd, float lineWidth )
{
    std::array<Vector2f, 4> out;
    
    float theta = std::atan2( lineEnd.y() - lineBegin.y(), lineEnd.x() - lineBegin.x() );
    float dx = lineWidth * std::sin(theta) / 2;
    float dy = lineWidth * std::cos(theta) / 2;
    
    Vector2f delta = { dx, dy };
    
    out[0] = lineBegin + delta; // TOP LEFT
    out[1] = lineEnd - delta; // BOTTOM RIGHT
    out[2] = lineBegin - delta; // BOTTOM LEFT
    out[3] = lineEnd + delta;   // BOTTOM RIGHT
    return out;
}

float Math::pointDistance( Vector2i p1, Vector2i p2 )
{
    int dx = p2.x() - p1.x();
    int dy = p2.y() - p1.y();
    return (float) std::sqrt( dx * dx + dy * dy );
}

Vector2i Math::centrePoint( Vector2i p1, Vector2i p2 )
{
    int dx = p2.x() - p1.x();
    int dy = p2.y() - p1.y();

    return Vector2i( p1.x() + dx / 2, p1.y() + dy / 2 );
}
