#include <ui/lib/TileHighlights.h>
#include <ui/lib/Manager.h>
#include <utils/GlobalConfig.h>
#include <graphics/RenderInterface.h>
#include <graphics/Primatives.h>
#include <game/Level.h>
#include <resource/ResourceManager.h>

using namespace UI; 

// Single Tile Highlight
// -----------------------

SingleTileHighlight::SingleTileHighlight(Manager* manager, Element* parent, Vector2i tile, SingleTileHighlightType type)
: Element( manager, parent )
{
    setPreferredContentSize({GlobalConfig::TileSizePx, GlobalConfig::TileSizePx});
    Sprite corners = ResourceManager::get().getSprite( "game_ui/tile-corners-white" );

    switch (type)
    {
        case SingleTileHighlightType::Green:
            corners.setPermanentColour(Colour::Green);
            break;
        case SingleTileHighlightType::Red:
            corners.setPermanentColour(Colour::Red);
            break;
        case SingleTileHighlightType::Yellow:
            corners.setPermanentColour(Colour::Yellow);
            break;
    }

    setBackground(corners);
    setLocalPosition( manager->level()->tileCoordsToScreen(tile) );
    setDecorative();
}

// Tile Region Highlight
// -----------------------

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


// Tile Arrow Highlight
// -----------------------

TileArrowHighlight::TileArrowHighlight( Manager *manager, Element *parent, OrderedGridRegion region, Colour colour, Vector2i origin )
    : Element(manager, parent)
{
    setDecorative();
    
    Assert( !region.empty() );

    if ( region.size() >= 2 )
    {
        region.insert( region.begin(), origin );

        for (int i = 1; i < region.size() - 1; i++)
        {
            Vector2i prev = region[i - 1];
            Vector2i curr = region[i];
            Vector2i next = region[i + 1];

            if ( GridUtils::isDiagonal(prev, curr) )
            {
                addDiagonalSegments(prev, curr);
            }

            addSpriteForTiles(region[i - 1], region[i], region[i + 1]);
        }

        Vector2i prev = region[region.size() - 2];
        Vector2i curr = region[region.size() - 1];
        addSpriteForTiles( prev, curr );
        
        if ( GridUtils::isDiagonal(prev, curr) )
        {
            addDiagonalSegments(prev, curr);
        }
    }
    else
    {
        Vector2i prev = origin;
        Vector2i curr = region.front();
        addSpriteForTiles( prev, curr );
    
        if ( GridUtils::isDiagonal(prev, curr) )
        {
            addDiagonalSegments(prev, curr);
        }
    }


    for ( auto& s : m_sprites )
    {
        s.setPermanentColour( Colour::Teal );
    }

}

void TileArrowHighlight::updateSelf( uint32_t ticks, InputInterface &iinter, RenderInterface &rInter )
{
    for (int i = 0; i < m_sprites.size(); i++)
    {
        rInter.addItem( m_sprites[i].renderObject( m_offsets[i] ), RenderLayer::Overlay);
    }
}

const std::vector<std::pair<Vector2i, Vector2i>> TileArrowHighlight::TriSegmentDeltas = {
        { Vector2i(0, -1), Vector2i(1, 0) },  // N_E,
        { Vector2i(0, -1), Vector2i(-1, 0) },  // N_W,
        { Vector2i(0, 1), Vector2i(-1, 0) },  // S_W,
        { Vector2i(0, 1), Vector2i(1, 0) },  // S_E,
        { Vector2i(0, -1), Vector2i(0, 1) },  // N_S,
        { Vector2i(1, 0), Vector2i(-1, 0) },  // E_W,
        { Vector2i(1, -1), Vector2i(-1, 1) },  // NE_SW,
        { Vector2i(-1, -1), Vector2i(1, 1) },  // NW_SE,
        { Vector2i(0, -1), Vector2i(-1, 1) },  // N_SW,
        { Vector2i(0, -1), Vector2i(1, 1) },  // N_SE,
        { Vector2i(1, 0), Vector2i(-1, -1) },  // E_NW,
        { Vector2i(1, 0), Vector2i(-1, 1) },  // E_SW,
        { Vector2i(0, 1), Vector2i(1, -1) },  // S_NE,
        { Vector2i(0, 1), Vector2i(-1, -1) },  // S_NW,
        { Vector2i(-1, 0), Vector2i(1, -1) },  // W_NE,
        { Vector2i(-1, 0), Vector2i(1, 1) },   // W_SE
        { Vector2i(-1, 1), Vector2i(1, 1) },   // NE_SE
        { Vector2i(-1, -1), Vector2i(-1, 1) },   // NW_SW
        { Vector2i(-1, -1), Vector2i(1, -1) },   // NW_NE
        { Vector2i(-1, 1), Vector2i(1, 1) },   // SW_SE
};

const std::vector<Vector2i> TileArrowHighlight::BiSegmentDeltas = {
        Vector2i(0, -1), // N
        Vector2i(1, 0), // E
        Vector2i(0, 1), // S
        Vector2i(-1, 0), // W
        Vector2i(1, -1), // NE
        Vector2i(1, 1), // SE
        Vector2i(-1, 1), // SW
        Vector2i(-1, -1), // NW
};

const std::vector<SpritesheetKey> TileArrowHighlight::TriSegmentSprites = {
        "game_ui/arrow-seg-n-e",
        "game_ui/arrow-seg-n-w",
        "game_ui/arrow-seg-s-w",
        "game_ui/arrow-seg-s-e",
        "game_ui/arrow-seg-n-s",
        "game_ui/arrow-seg-w-e",
        "game_ui/arrow-seg-sw-ne",
        "game_ui/arrow-seg-nw-se",
        "game_ui/arrow-seg-n-sw",
        "game_ui/arrow-seg-n-se",
        "game_ui/arrow-seg-e-nw",
        "game_ui/arrow-seg-e-sw",
        "game_ui/arrow-seg-s-ne",
        "game_ui/arrow-seg-s-nw",
        "game_ui/arrow-seg-w-ne",
        "game_ui/arrow-seg-w-se",
        "game_ui/arrow-seg-ne-se",
        "game_ui/arrow-seg-sw-nw",
        "game_ui/arrow-seg-nw-ne",
        "game_ui/arrow-seg-sw-se",
};

const std::vector<SpritesheetKey> TileArrowHighlight::BiSegmentSprites = {
        "game_ui/arrow-seg-n", // N
        "game_ui/arrow-seg-e", // E
        "game_ui/arrow-seg-s", // S
        "game_ui/arrow-seg-w", // W
        "game_ui/arrow-seg-ne", // NE
        "game_ui/arrow-seg-se", // SE
        "game_ui/arrow-seg-sw", // SW
        "game_ui/arrow-seg-nw", // NW
};

const std::vector<Vector2i> TileArrowHighlight::TriSegmentOffsets = {
        Vector2i(0, 0), // N_E,
        Vector2i(0, 0), // N_W,
        Vector2i(0, 0), // S_W,
        Vector2i(0, 0), // S_E,
        Vector2i(8, 0), // N_S,
        Vector2i(0, 8), // E_W,
        Vector2i(0, 0), // NE_SW,
        Vector2i(0, 0), // NW_SE,
        Vector2i(0, 0), // N_SW,
        Vector2i(8, 0), // N_SE,
        Vector2i(0, 0), // E_NW,
        Vector2i(0, 8), // E_SW,
        Vector2i(8, 0), // S_NE,
        Vector2i(0, 0), // S_NW,
        Vector2i(0, 0), // W_NE,
        Vector2i(0, 8), // W_SE
        Vector2i(0, 0), // NE_SE
        Vector2i(-1, 0), // NW_SW
        Vector2i(0, 0), // NW_NE
        Vector2i(0, 0), // SW_SE
        
};

const std::vector<Vector2i> TileArrowHighlight::BiSegmentOffsets = {
        Vector2i(8, 0), // N
        Vector2i(8, 8), // E
        Vector2i(8, 8), // S
        Vector2i(0, 8), // W
        Vector2i(11, -2), // NE
        Vector2i(11, 11), // SE
        Vector2i(0, 10), // SW
        Vector2i(-1, 0), // NW
};

void TileArrowHighlight::addSpriteForTiles(Vector2i prev, Vector2i curr, Vector2i next )
{
    TriArrowSegment segment = segmentFromTiles(prev, curr, next);
    auto& key = TriSegmentSprites[(int)segment];

    Sprite s = ResourceManager::get().getSprite(key);
    m_sprites.push_back( s );
    m_offsets.push_back( (curr * GlobalConfig::TileSizePx) + TriSegmentOffsets[(int)segment]);
}

void TileArrowHighlight::addSpriteForTiles(Vector2i prev, Vector2i curr)
{
    BiArrowSegment segment = segmentFromTiles(prev, curr);
    auto& key = BiSegmentSprites[(int)segment];

    Sprite s = ResourceManager::get().getSprite(key);
    m_sprites.push_back( s );
    m_offsets.push_back( (curr * GlobalConfig::TileSizePx) + BiSegmentOffsets[(int)segment]);
}


TriArrowSegment TileArrowHighlight::segmentFromTiles(Vector2i prev, Vector2i curr, Vector2i next)
{
    Vector2i forward = next - curr;
    Vector2i backward = prev - curr;

    for (int i = 0; i < TriSegmentDeltas.size(); i++)
    {
        auto const&[a, b] = TriSegmentDeltas[i];
        if ((a == forward && b == backward) || (b == forward && a == backward))
        {
            return (TriArrowSegment) i;
        }
    }

    // Hopefully not possible?
//    Logging::log("Weird segment: prev:{}, curr:{}, next:{}\n", prev.to_string(), curr.to_string(), next.to_string());
    return TriArrowSegment::N_S;
}

BiArrowSegment TileArrowHighlight::segmentFromTiles(Vector2i curr, Vector2i next)
{
    Vector2i forward = curr - next;

    for (int i = 0; i < BiSegmentDeltas.size(); i++)
    {
        auto const& a = BiSegmentDeltas[i];
        if (a == forward)
        {
            return (BiArrowSegment) i;
        }
    }

    // Hopefully not possible?
    Logging::log("Weird segment: curr:{}, next:{}\n", curr.to_string(), next.to_string());
    return BiArrowSegment::N;
}

void TileArrowHighlight::addDiagonalSegments(Vector2i first, Vector2i second)
{
    if (first.x() > second.x() )
    {
        std::swap(first, second);
    }

    if ( first.y() < second.y() )
    {
        auto ne = ResourceManager::get().getSprite( "game_ui/arrow-seg-fill-ne" );
        m_sprites.push_back( ne );
        m_offsets.push_back( (first + Vector2i{0, 1}) * GlobalConfig::TileSizePx + Vector2(20, 0));

        auto sw = ResourceManager::get().getSprite( "game_ui/arrow-seg-fill-sw" );
        m_sprites.push_back( sw );
        m_offsets.push_back( (first + Vector2i{1, 0}) * GlobalConfig::TileSizePx+ Vector2(0, 20));
    }
    else
    {
        auto nw = ResourceManager::get().getSprite( "game_ui/arrow-seg-fill-nw" );
        m_sprites.push_back( nw );
        m_offsets.push_back( (first + Vector2i{1, 0}) * GlobalConfig::TileSizePx);

        auto se = ResourceManager::get().getSprite( "game_ui/arrow-seg-fill-se" );
        m_sprites.push_back( se );
        m_offsets.push_back( (first + Vector2i{0, -1}) * GlobalConfig::TileSizePx + Vector2(19, 19));
    }
}


