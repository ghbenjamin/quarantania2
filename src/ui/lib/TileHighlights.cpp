#include <ui/lib/TileHighlights.h>
#include <ui/lib/Manager.h>
#include <utils/GlobalConfig.h>
#include <graphics/RenderInterface.h>
#include <graphics/Primatives.h>
#include <game/Level.h>
#include <resource/ResourceManager.h>

using namespace UI; 

SingleTileHighlight::SingleTileHighlight(Manager* manager, Element* parent, Vector2i tile, SingleTileHighlightType type)
: Element( manager, parent )
{
    setPreferredContentSize({GlobalConfig::TileSizePx, GlobalConfig::TileSizePx});

    switch (type)
    {
        case SingleTileHighlightType::Green:
            setBackground( SpritesheetKey{"game_ui/tile-corners-green"});
            break;
        case SingleTileHighlightType::Red:
            setBackground( SpritesheetKey{"game_ui/tile-corners-red"});
            break;
        case SingleTileHighlightType::Yellow:
            setBackground( SpritesheetKey{"game_ui/tile-corners-yellow"});
            break;
    }

    setLocalPosition( manager->level()->tileCoordsToScreen(tile) );
    setDecorative();
}


TileRegionHighlight::TileRegionHighlight(Manager *manager, Element *parent, GridRegion region, Colour colour)
    : Element(manager, parent), m_region(region)
{
    setDecorative();

    m_sprite = createRectangle({GlobalConfig::TileSizePx, GlobalConfig::TileSizePx}, colour.withAlpha(100));
    m_sprite.setRenderLayer(RenderLayer::Overlay);
}


void TileRegionHighlight::updateSelf(uint32_t ticks, InputInterface &iinter, RenderInterface &rInter)
{
    for ( auto const& tile : m_region )
    {
       rInter.addItem(m_sprite.renderObject( tile * GlobalConfig::TileSizePx ), RenderLayer::Overlay);
    }
}


TileArrowHighlight::TileArrowHighlight( Manager *manager, Element *parent, OrderedGridRegion region, Colour colour, Vector2i origin )
    : Element(manager, parent)
{
    setDecorative();
    
    AssertMsg( region.size() >= 2, "Arrow highlight must have at least two tiles" );
    
    std::vector<SpritesheetKey> keys;
    
    keys.push_back( getSpriteForTiles(region[0], region[1]) );
    
    for (int i = 1; i < region.size() - 1; i++)
    {
        keys.push_back( getSpriteForTiles( region[i-1], region[i], region[i+1] ) );
    }
    
    keys.push_back( getSpriteForTiles(region[region.size() - 2], region[region.size() - 1]) );
    
    
    for (int i = 0; i < region.size(); i++)
    {
        Sprite s = ResourceManager::get().getSprite(keys[i]);
        m_sprites.push_back( s );
        m_offsets.push_back(  region[i] * GlobalConfig::TileSizePx );
    }
}

void TileArrowHighlight::updateSelf( uint32_t ticks, InputInterface &iinter, RenderInterface &rInter )
{
    for (int i = 0; i < m_sprites.size(); i++)
    {
        rInter.addItem( m_sprites[i].renderObject( m_offsets[i] ), RenderLayer::Overlay);
    }
}

SpritesheetKey TileArrowHighlight::getSpriteForTiles( Vector2i prev, Vector2i curr, Vector2i next )
{
    Vector2i forward = next - curr;
    Vector2i backward = prev - curr;
    
    // Straights

    if ( (forward == Vector2i{1, 0} && backward == Vector2i{-1, 0})
        || (forward == Vector2i{-1, 0} && backward == Vector2i{1, 0}) )
    {
        return "game_ui/arrow-seg-w-e";
    }
    else if ( (forward == Vector2i{0, 1} && backward == Vector2i{0, -1})
        || (forward == Vector2i{0, -1} && backward == Vector2i{0, 1}) )
    {
        return "game_ui/arrow-seg-n-s";
    }

    // Diagonals

    else if ( (forward == Vector2i{1, 1} && backward == Vector2i{-1, -1})
              || (forward == Vector2i{-1, -1} && backward == Vector2i{1, 1}) )
    {
        return "game_ui/arrow-seg-nw-se";
    }

    else if ( (forward == Vector2i{-1, 1} && backward == Vector2i{1, -1})
              || (forward == Vector2i{1, -1} && backward == Vector2i{-1, 1}) )
    {
        return "game_ui/arrow-seg-sw-ne";
    }
    
    // Mixed
    
    else if (( forward == Vector2i{ 1, 0 } && backward == Vector2i{ -1, -1 } )
        || ( forward == Vector2i{ -1, -1 } && backward == Vector2i{ 1, 0 } ))
    {
        return "game_ui/arrow-seg-e-nw";
    }
    else if (( forward == Vector2i{ 0, -1 } && backward == Vector2i{ -1, 1 } )
        || ( forward == Vector2i{ -1, 1 } && backward == Vector2i{ 0, -1 } ))
    {
        return "game_ui/arrow-seg-n-sw";
    }
    else if (( forward == Vector2i{ 0, 1 } && backward == Vector2i{ 1, -1 } )
        || ( forward == Vector2i{ 1, -1 } && backward == Vector2i{ 0, 1 } ))
    {
        return "game_ui/arrow-seg-s-ne";
    }
    else if (( forward == Vector2i{ -1, 0 } && backward == Vector2i{ 1, 1 } )
        || ( forward == Vector2i{ 1, 1 } && backward == Vector2i{ -1, 1 } ))
    {
        return "game_ui/arrow-seg-w-se";
    }

    
    return "game_ui/walk";
}

SpritesheetKey TileArrowHighlight::getSpriteForTiles( Vector2i curr, Vector2i next )
{
    Vector2i forward = next - curr;
    
    if ( forward == Vector2i(1, 0) )
    {
        return "game_ui/arrow-seg-e";
    }
    else if ( forward == Vector2i(-1, 0) )
    {
        return "game_ui/arrow-seg-w";
    }
    else if ( forward == Vector2i(0, 1) )
    {
        return "game_ui/arrow-seg-n";
    }
    else if ( forward == Vector2i(0, -1) )
    {
        return "game_ui/arrow-seg-s";
    }
    
    return "game_ui/walk";
}

