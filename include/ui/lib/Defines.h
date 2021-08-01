#pragma once

#include <optional>
#include <utils/Colour.h>
#include <utils/Containers.h>

class FtFontFace;

namespace UI
{

struct TextStyle
{
    Colour textColour;
    std::shared_ptr<FtFontFace> font;
    int fontSize;
};

}