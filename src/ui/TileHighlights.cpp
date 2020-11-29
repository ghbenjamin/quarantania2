#include <ui/TileHighlights.h>
#include <ui/Manager.h>
#include <utils/GlobalConfig.h>
#include <graphics/RenderInterface.h>
#include <graphics/Primatives.h>
#include <game/Level.h>

UI::SingleTileHighlight::SingleTileHighlight(Manager* manager, Element* parent, Vector2i tile)
: Element( manager, parent )
{
    setPreferredContentSize({GlobalConfig::TileSizePx, GlobalConfig::TileSizePx});
    setBackgroundSprite({"tile-ui", "green-brackets"});
    setLocalPosition( manager->level()->tileCoordsToScreen(tile) );
    setDecorative();
}

UI::TileRegionHighlight::TileRegionHighlight(UI::Manager *manager, UI::Element *parent, GridRegion region, Colour colour)
    : Element(manager, parent), m_region(region)
{
    setDecorative();

    m_sprite = createRectangle({GlobalConfig::TileSizePx, GlobalConfig::TileSizePx}, colour.withAlpha(100));
    m_sprite.setRenderLayer(RenderLayer::OVERLAY_UI);
}


void UI::TileRegionHighlight::updateSelf(uint32_t ticks, InputInterface &iinter, RenderInterface &rInter)
{
    for ( auto const& tile : m_region )
    {
       rInter.addWorldItem(m_sprite.renderObject( tile * GlobalConfig::TileSizePx ));
    }
}


