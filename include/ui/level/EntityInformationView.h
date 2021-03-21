#pragma once

#include <ui/lib/Element.h>
#include <engine/Entity.h>

namespace UI
{

class EntityInformationView : public Element
{
public:
    EntityInformationView(Manager* manager, Element* parent);
    ~EntityInformationView() override = default;
    
    void refresh(EntityRef entity);
};


}