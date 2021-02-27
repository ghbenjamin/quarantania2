#pragma once

#include <ui/lib/Element.h>
#include <ui/lib/Defines.h>

namespace UI
{

static inline const int TOOLTIP_MAX_WIDTH = 300;
static inline const int TOOLTIP_MIN_WIDTH = 150;


class TooltipItem : public Element
{
public:
    TooltipItem(Manager* manager, Element* parent, TooltipData const& data );
    ~TooltipItem() override = default;
};


class Tooltip : public Element
{
public:

    Tooltip(Manager* manager, Element* parent, TooltipData const& data );
    Tooltip(Manager* manager, Element* parent, std::vector<TooltipData> const& data );
    ~Tooltip() override = default;

private:
    void sharedInit();
};

}