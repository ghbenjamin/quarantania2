#pragma once

#include <utils/Containers.h>

namespace UI
{

enum class Alignment
{
    TopLeft,
    TopCentre,
    TopRight,
    CentreRight,
    BottomRight,
    BottomCentre,
    BottomLeft,
    CentreLeft,
    Centre
};

Vector2i alignRectWithinRect( Vector2i const& outer, Vector2i const& inner, Alignment alignment );

}