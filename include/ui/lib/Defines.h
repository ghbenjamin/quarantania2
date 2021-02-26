#pragma once

#include <optional>
#include <utils/Colour.h>
#include <utils/Containers.h>
#include <resource/Font.h>

namespace UI
{

struct TextStyle
{
    Colour textColour;
    FontDataPtr font;
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