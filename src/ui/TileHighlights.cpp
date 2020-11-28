#include <ui/TileHighlights.h>
#include <utils/GlobalConfig.h>

UI::SingleTileHighlight::SingleTileHighlight(Manager* manager, Element* parent)
: Element( manager, parent )
{
    setPreferredContentSize({GlobalConfig::TileSizePx, GlobalConfig::TileSizePx});
    setBackgroundSprite({"tile-ui", "green-brackets"});
    setId("tile-highlight");
}



