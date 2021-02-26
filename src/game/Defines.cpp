#include <game/Defines.h>
#include <ui/lib/Defines.h>

UI::TooltipData::TooltipData( std::string const & title )
 : title(title) {}

UI::TooltipData::TooltipData( std::string const &title, std::string const &subtitle, std::string const& content )
 : title(title), subtitle(subtitle), content(content) {}
