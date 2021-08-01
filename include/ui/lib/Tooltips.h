#pragma once

#include <ui/lib/Element.h>
#include <ui/lib/Defines.h>

namespace UI
{

static inline const int TOOLTIP_MAX_WIDTH = 300;
static inline const int TOOLTIP_MIN_WIDTH = 150;


class Tooltip : public Element
{
public:
    
    Tooltip( Manager* manager, Element* parent );
    ~Tooltip() override = default;
};

class SimpleTooltipItem : public Element
{
public:
    SimpleTooltipItem( Manager* manager, Element* parent, std::string const& title, std::string const& content );
    ~SimpleTooltipItem() override = default;
};

class SimpleTooltip : public Tooltip
{
public:
    SimpleTooltip(Manager* manager, Element* parent, std::string const& title, std::string const& content );
    ~SimpleTooltip() override = default;
};


}