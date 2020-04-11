#pragma once

#include <ui/Element.h>
#include <ui/Defines.h>

namespace UI
{

class Tooltip : public Element
{
public:
    Tooltip(Manager* manager, Element* parent, TooltipData const& data );
    ~Tooltip() override = default;

private:
    static const int TOOLTIP_WIDTH = 300;

};

}