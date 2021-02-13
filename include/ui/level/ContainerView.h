#pragma once

#include <ui/lib/Element.h>
#include <engine/Entity.h>

struct ContainerComponent;
class Item;

namespace UI
{

struct ContainerViewItem
{
    Sprite sprite;
    std::shared_ptr<Item> item;
};

// A view into a container of items, e.g. a chest or the inventory of an actor
class ContainerView : public Element
{

public:
    ContainerView(Manager* manager, Element* parent, Vector2i iconDims);
    ~ContainerView() override = default;

    void refresh(EntityRef entity);

private:

    void updateSelf(uint32_t ticks, InputInterface &iinter, RenderInterface &rInter) override;
    void onClick( UMouseButtonEvent& evt );


private:

    EntityRef m_entity;
    std::vector<ContainerViewItem> m_items;
    Vector2i m_iconDims;
    Sprite m_emptySlot;
    int m_iconSpacing;
};



}