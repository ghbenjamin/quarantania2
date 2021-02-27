#pragma once

#include <ui/lib/Element.h>
#include <engine/Entity.h>

namespace UI
{

class Label;

class PlayerStatusWidget : public Element
{
public:
    PlayerStatusWidget(Manager* manager, Element* parent, EntityRef entity );
    ~PlayerStatusWidget() override = default;
    
    void refresh();

private:
    
    void highlightEntity();
    void unhighlightEntity();
    void selectEntity();
    
    
    EntityRef m_entity;
    std::shared_ptr<Label> m_nameLabel;
};


class PlayerStatusContainer : public Element
{
public:
    PlayerStatusContainer(Manager* manager, Element* parent );
    ~PlayerStatusContainer() override = default;
    
    void reloadEntities();
    void refresh();
};

}