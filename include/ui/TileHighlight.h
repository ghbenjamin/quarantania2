#pragma once

#include <ui/Element.h>

namespace UI
{

class TileHighlight : public Element
{

    TileHighlight(Manager* manager, Element* parent);
public:
    virtual ~TileHighlight() = default;
};

}