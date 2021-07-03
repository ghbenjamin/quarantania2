#pragma once

#include <ui/lib/Element.h>

#include <engine/Entity.h>

struct ContainerComponent;
class Item;
class Level;

namespace UI
{

class Icon;

class ContainerViewItem : public Element
{
public:
    ContainerViewItem(Manager* manager, Element* parent, Level* level, EntityRef entity, Sprite const& backgroundSprite);

    void setItem( std::shared_ptr<Item> item );
    void setEntity( EntityRef entity );
    void reset();

private:

    void onClick();
    std::optional<TooltipData> tooltipSpawner();
    
    Level* m_level;
    std::shared_ptr<Icon> m_icon;
    std::shared_ptr<Item> m_item;
    EntityRef m_entity;
};


// A view into a container of items, e.g. a chest or the inventory of an actor
class ContainerView : public Element
{

public:
    ContainerView(Manager* manager, Element* parent, Level* level, Vector2i iconDims);
    ~ContainerView() override = default;

    void refresh(EntityRef entity);

private:
    Level* m_level;
    EntityRef m_entity;
    std::vector<std::shared_ptr<ContainerViewItem>> m_items;
    Vector2i m_iconDims;
};



}