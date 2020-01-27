#pragma once

#include <ui/Element.h>
#include <game/Entity.h>

struct ContainerComponent;

namespace UI
{

struct ContainerViewItem
{
    Sprite sprite;
};

// A view into a container of items, e.g. a chest or the inventory of an actor
class ContainerView : public Element
{

public:
    ContainerView( );
    ~ContainerView() override = default;

    void attachContainer( std::shared_ptr<::ContainerComponent> container );
    void reimportItems();

private:

    void updateSelf(uint32_t ticks, InputInterface &iinter, RenderInterface &rInter) override;
    void onSizeSelf() override;
    void onMoveSelf() override;

    void rearrangeItems();

    std::shared_ptr<ContainerComponent> m_container;
    std::vector<ContainerViewItem> m_items;
    Vector2i m_tileBounds;

    static const int IconSize = 32;
    static const int PaddingThick = 2;

    Sprite m_emptySlot;
};

}