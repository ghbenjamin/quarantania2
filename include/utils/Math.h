#pragma once

#include <array>
#include <vector>
#include <utils/Containers.h>

namespace Math
{

Vector2f findCentroid( std::vector<Vector2i> points );
std::array<Vector2f, 4> getQuadForLine( Vector2f lineBegin, Vector2f lineEnd, float lineWidth );

}
