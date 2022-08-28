#pragma once

#include <ui/lib/Element.h>

class Level;
struct ActorCalcData;

namespace UI
{

class ModifierOverlay : public Element
{
public:
    
    ModifierOverlay(Manager* manager, Element* parent, Level* level, ActorCalcData const& data);
    ~ModifierOverlay() override = default;

private:
    Level* m_level;

};

}