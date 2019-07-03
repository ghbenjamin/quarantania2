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

    ElementStyle()
        : hasBgColour(false), hasBorder(false), borderWidth(0) {}
    ~ElementStyle() = default;

};

struct TextStyle
{
    Colour textColour;
};


}