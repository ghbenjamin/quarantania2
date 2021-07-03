#pragma once

#include <ui/lib/Element.h>

#include <utils/GridUtils.h>

class Level;

namespace UI
{

enum class SingleTileHighlightType
{
    Green,
    Red,
    Yellow
};

class SingleTileHighlight : public Element
{
public:
    SingleTileHighlight(Manager* manager, Element* parent, Vector2i screenCoords, SingleTileHighlightType type);
    ~SingleTileHighlight() override = default;
};



class TileRegionHighlight : public Element
{
public:
    TileRegionHighlight(Manager* manager, Element* parent, GridRegion region, Colour colour);
    ~TileRegionHighlight() override = default;

private:
    void updateSelf(uint32_t ticks, InputInterface &iinter, RenderInterface &rInter) override;
    
    RenderObject m_robj;
};



enum class TriArrowSegment
{
    N_E,
    N_W,
    S_W,
    S_E,
    N_S,
    E_W,
    NE_SW,
    NW_SE,
    N_SW,
    N_SE,
    E_NW,
    E_SW,
    S_NE,
    S_NW,
    W_NE,
    W_SE
};

enum class BiArrowSegment
{
    N,
    E,
    S,
    W,
    NE,
    SE,
    SW,
    NW
};


class TileArrowHighlight : public Element
{
public:
    TileArrowHighlight(Manager* manager, Element* parent, OrderedGridRegion region, Colour colour, Vector2i origin);
    ~TileArrowHighlight() override = default;

private:

    static const std::vector<std::pair<Vector2i, Vector2i>> TriSegmentDeltas;
    static const std::vector<SpritesheetKey> TriSegmentSprites;
    static const std::vector<Vector2i> TriSegmentOffsets;
    static const std::vector<Vector2i> BiSegmentDeltas;
    static const std::vector<SpritesheetKey> BiSegmentSprites;
    static const std::vector<Vector2i> BiSegmentOffsets;

    static TriArrowSegment segmentFromTiles(Vector2i prev, Vector2i curr, Vector2i next);
    static BiArrowSegment segmentFromTiles(Vector2i curr, Vector2i next);

    void updateSelf(uint32_t ticks, InputInterface &iinter, RenderInterface &rInter) override;
    void addSpriteForTiles(Vector2i prev, Vector2i curr, Vector2i next );
    void addSpriteForTiles( Vector2i curr, Vector2i next );

    void addDiagonalSegments( Vector2i first, Vector2i second );
    
    std::vector<Sprite> m_sprites;
    std::vector<Vector2i> m_offsets;
};


}