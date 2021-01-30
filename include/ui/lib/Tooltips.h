#pragma once

#include <ui/lib/Element.h>
#include <ui/lib/Defines.h>

namespace UI
{

static inline constexpr int TOOLTIP_MAX_WIDTH = 250;


class TooltipItem : public Element
{
public:
    TooltipItem(Manager* manager, Element* parent, TooltipData const& data, bool longContent=false );
    ~TooltipItem() override = default;

};


class Tooltip : public Element
{
public:

    Tooltip(Manager* manager, Element* parent, TooltipData const& data, bool longContent=false );
    Tooltip(Manager* manager, Element* parent, std::vector<TooltipData> const& data, bool longContent=false );
    ~Tooltip() override = default;

private:
    void sharedInit();
};

}