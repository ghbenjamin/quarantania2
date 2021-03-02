#pragma once

#include <ui/lib/Element.h>

namespace UI
{

class BottomLeftBar : public Element
{
public:
    BottomLeftBar(Manager* manager, Element* parent);
    ~BottomLeftBar() override = default;

};

}