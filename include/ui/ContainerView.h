#pragma once

#include <ui/Element.h>
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
    ContainerView(Manager* manager, Element* parent);
    ~ContainerView() override = default;

    void attachEntity(EntityRef entity);
    void reimportItems();

private:

    void updateSelf(uint32_t ticks, InputInterface &iinter, RenderInterface &rInter) override;
    void onSizeSelf() override;
    void onMoveSelf() override;

    void onClick( UMouseButtonEvent& evt );
    void onMouseMove( UMouseMoveEvent& evt );

    ContainerViewItem const* itemFromPosition( Vector2i position ) const;

private:

    EntityRef m_entity;
    std::vector<ContainerViewItem> m_items;
    Vector2i m_tileBounds;
    Vector2i m_tileOffset;

    static const int IconSize = 38;
    static const int PaddingThick = 4;

    Sprite m_emptySlot;
    ContainerViewItem const* m_hoveredItem;
};

}