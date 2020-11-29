#pragma once

#include <ui/Element.h>
#include <utils/GridUtils.h>

namespace UI
{

class SingleTileHighlight : public Element
{
public:
    SingleTileHighlight(Manager* manager, Element* parent, Vector2i tile);
    virtual ~SingleTileHighlight() = default;
};


class TileRegionHighlight : public Element
{
public:
    TileRegionHighlight(Manager* manager, Element* parent, GridRegion region, Colour colour);
    virtual ~TileRegionHighlight() override = default;

private:
    void updateSelf(uint32_t ticks, InputInterface &iinter, RenderInterface &rInter) override;

    GridRegion m_region;
    Sprite m_sprite;
};

}