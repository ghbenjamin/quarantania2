#pragma once

#include <utils/Colour.h>
#include <utils/Containers.h>
#include <resource/Font.h>

namespace UI
{

struct TextStyle
{
    Colour textColour;
    FontPtr font;
};

struct TooltipData
{
    std::string title;
    std::string subtitle;
    std::string content;
};

}