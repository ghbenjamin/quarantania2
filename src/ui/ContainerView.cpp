#include <ui/ContainerView.h>
#include <ui/Manager.h>
#include <graphics/Primatives.h>
#include <graphics/RenderInterface.h>
#include <components/ContainerComponent.h>
#include <resource/ResourceManager.h>
#include <game/Level.h>
#include <actions/ActionDefs.h>

UI::ContainerView::ContainerView()
: m_emptySlot( createRectangle( {IconSize, IconSize}, Colour::Black ) ),
  m_hoveredItem(nullptr)
{
    setBackgroundColour({200, 200, 200, 255});
    m_emptySlot.setRenderLayer(RenderLayer::UI);

    addEventCallback(UEventType::Click, [this](UEvent& evt){
        onClick( evt.mouseButtonEvent );
    });

    addEventCallback(UEventType::MouseMove, [this](UEvent& evt){
        onMouseMove( evt.mouseMoveEvent );
    });
}

void UI::ContainerView::updateSelf(uint32_t ticks, InputInterface &iinter, RenderInterface &rInter)
{
    int idx = 0;
    Vector2i offset = globalPosition() + contentOffset();
    Vector2i curr = {PaddingThick, PaddingThick};

    for ( int j = 0; j < m_tileBounds.y(); j++ )
    {
        for ( int i = 0; i < m_tileBounds.x(); i++ )
        {
            if (idx < m_items.size())
            {
                rInter.addScreenItem( m_items[idx].sprite.renderObject(curr + offset) );
            }
            else
            {
                rInter.
                addScreenItem( m_emptySlot.renderObject(curr + offset) );
            }

            curr.x( curr.x() + IconSize + PaddingThick );
            idx++;
        }

        curr.y( curr.y() + IconSize + PaddingThick );
        curr.x( PaddingThick );
    }
}

void UI::ContainerView::onSizeSelf()
{
    rearrangeItems();
}

void UI::ContainerView::onMoveSelf()
{
    rearrangeItems();
}

void UI::ContainerView::rearrangeItems()
{
    m_tileBounds = {
        (innerBounds().w() - PaddingThick) / (IconSize + PaddingThick),
        (innerBounds().h() - PaddingThick) / (IconSize + PaddingThick)
    };
}

void UI::ContainerView::reimportItems()
{
    auto container = manager()->level()->getComponents<ContainerComponent>(m_entity);

    m_items.clear();
    for ( auto const& item : container->items )
    {
        auto sprite = ResourceManager::get().getSprite( item->data()->sprite );
        sprite.setRenderLayer(RenderLayer::UI);
        m_items.push_back( ContainerViewItem{sprite, item} );
    }
}

void UI::ContainerView::attachEntity(EntityRef entity)
{
    m_entity = entity;
}

void UI::ContainerView::onMouseMove(UI::UMouseMoveEvent& evt)
{
    auto selectedItem = itemFromPosition( evt.pos - globalPosition() - contentOffset() );

    if ( selectedItem == nullptr )
    {
        if ( m_hoveredItem != nullptr )
        {
            // hover out
        }
    }
    else
    {
        if ( m_hoveredItem != nullptr )
        {
            if ( m_hoveredItem == selectedItem )
            {
                // Nothing
            }
            else
            {
                // Hover change
            }
        }
        else
        {
            // Hover in
        }
    }

    m_hoveredItem = selectedItem;
}

void UI::ContainerView::onClick(UI::UMouseButtonEvent& evt)
{
    if ( evt.button == UI::UMouseButtonEvent::LeftMouseButton )
    {
        auto selectedItem = itemFromPosition( evt.pos - globalPosition() - contentOffset() );
        if ( selectedItem != nullptr )
        {
            auto equipAction = std::make_shared<EquipItemAction>( manager()->level(), m_entity, selectedItem->item );
            equipAction->doAction();
        }
    }
    else if ( evt.button == UI::UMouseButtonEvent::RightMouseButton )
    {
        auto selectedItem = itemFromPosition( evt.pos - globalPosition() - contentOffset() );
        if ( selectedItem != nullptr )
        {
            auto dropAction = std::make_shared<DropItemAction>( manager()->level(), m_entity, selectedItem->item );
            dropAction->doAction();
        }
    }
}

UI::ContainerViewItem const* UI::ContainerView::itemFromPosition(Vector2i position) const
{
    position -= { PaddingThick, PaddingThick };
    Vector2i tileCoords = position / ( IconSize + PaddingThick );

    int idx = tileCoords.x() + m_tileBounds.x() * tileCoords.y();

    if ( idx < m_items.size() )
    {
        return &m_items[idx];
    }

    return nullptr;
}