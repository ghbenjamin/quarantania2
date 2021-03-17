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

struct TooltipData
{
    TooltipData( std::string const& title );
    TooltipData( std::string const& title, std::string const& subtitle, std::string const& content );

    std::string title;
    std::optional<std::string> subtitle;
    std::optional<std::string> content;
};

}