#pragma once

#include <ui/lib/Element.h>

namespace UI
{

class BottomMenubar : public Element
{
public:
    BottomMenubar(Manager* manager, Element* parent);
    ~BottomMenubar() override = default;
    
};

}