#include <ui/TileHighlight.h>

UI::TileHighlight::TileHighlight(Manager* manager, Element* parent)
: Element( manager, parent )
{
    setBackgroundColour( Colour::Lavender.withAlpha( 100 ) );
    setId("tile-highlight");
}



