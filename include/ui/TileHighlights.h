#pragma once

#include <ui/Element.h>

namespace UI
{

class SingleTileHighlight : public Element
{
public:
    SingleTileHighlight(Manager* manager, Element* parent);
    virtual ~SingleTileHighlight() = default;
};

}