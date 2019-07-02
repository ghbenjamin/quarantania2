#pragma once

#include <utils/Colour.h>
#include <utils/Containers.h>

namespace UI
{


struct ElementStyle
{
    bool hasBgColour;
    bool hasBorder;

    Colour bgColour;
    Colour borderColour;
    int borderWidth;

    RectI padding;
};

struct TextStyle
{
    Colour textColour;
};


}