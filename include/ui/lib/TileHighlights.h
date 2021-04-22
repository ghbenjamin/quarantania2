#pragma once

#include <ui/lib/Element.h>
#include <utils/GridUtils.h>

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
    SingleTileHighlight(Manager* manager, Element* parent, Vector2i tile, SingleTileHighlightType type);
    ~SingleTileHighlight() override = default;
};



class TileRegionHighlight : public Element
{
public:
    TileRegionHighlight(Manager* manager, Element* parent, GridRegion region, Colour colour);
    ~TileRegionHighlight() override = default;

private:
    void updateSelf(uint32_t ticks, InputInterface &iinter, RenderInterface &rInter) override;

    GridRegion m_region;
    Sprite m_sprite;
};



class TileArrowHighlight : public Element
{
public:
    TileArrowHighlight(Manager* manager, Element* parent, OrderedGridRegion region, Colour colour, Vector2i origin);
    ~TileArrowHighlight() override = default;

private:
    
    void updateSelf(uint32_t ticks, InputInterface &iinter, RenderInterface &rInter) override;
    SpritesheetKey getSpriteForTiles( Vector2i prev, Vector2i curr, Vector2i next );
    SpritesheetKey getSpriteForTiles( Vector2i curr, Vector2i next );
    
    std::vector<Sprite> m_sprites;
    std::vector<Vector2i> m_offsets;
};


}