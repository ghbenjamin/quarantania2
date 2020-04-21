#include <ui/ContainerView.h>
#include <ui/Manager.h>
#include <graphics/Primatives.h>
#include <graphics/RenderInterface.h>
#include <components/ContainerComponent.h>
#include <resource/ResourceManager.h>
#include <game/Level.h>
#include <actions/ActionDefs.h>
#include <utils/GlobalConfig.h>

UI::ContainerView::ContainerView(Manager* manager, Element* parent)
: Element(manager, parent),
  m_hoveredItem(nullptr), m_tileBounds{8, 4}, m_tileOffset{4, 22}
{
    m_emptySlot = ResourceManager::get().getSprite("ui-frames", "inventory-slot");
    m_emptySlot.setRenderLayer(RenderLayer::UI);

    addEventCallback(UEventType::Click, [this](UEvent& evt){
        onClick( evt.mouseButtonEvent );
    });

    addEventCallback(UEventType::MouseMove, [this](UEvent& evt){
        onMouseMove( evt.mouseMoveEvent );
    });

    addEventCallback(UEventType::MouseOut, [this](UEvent& evt){
        // Stop leaving tooltips about
        this->manager()->closeTooltip();
    });
}

void UI::ContainerView::updateSelf(uint32_t ticks, InputInterface &iinter, RenderInterface &rInter)
{
    int idx = 0;
    Vector2i offset = globalPosition() + contentOffset() + m_tileOffset;
    Vector2i curr = {PaddingThick, PaddingThick};

    int innerOffsetW = (IconSize - GlobalConfig::TileSizePx) / 2;
    Vector2i innerOffset = { innerOffsetW, innerOffsetW };

    for ( int j = 0; j < m_tileBounds.y(); j++ )
    {
        for ( int i = 0; i < m_tileBounds.x(); i++ )
        {
            if ( idx < (int) m_items.size() )
            {
                rInter.addScreenItem( m_items[idx].sprite.renderObject(curr + offset + innerOffset) );
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
}

void UI::ContainerView::onMoveSelf()
{
}

void UI::ContainerView::reimportItems()
{
    auto container = manager()->level()->getComponents<ContainerComponent>(m_entity);

    m_items.clear();
    for ( auto const& item : container->items )
    {
        auto sprite = ResourceManager::get().getSprite( item->getSprite() );
        sprite.setRenderLayer(RenderLayer::UI);
        m_items.push_back( ContainerViewItem{sprite, item} );
    }
}

void UI::ContainerView::attachEntity(EntityRef entity)
{
    m_entity = entity;
    reimportItems();
}

void UI::ContainerView::onMouseMove(UI::UMouseMoveEvent& evt)
{
    auto selectedItem = itemFromPosition( evt.pos - globalPosition() - contentOffset() );

    if ( selectedItem == nullptr )
    {
        if ( m_hoveredItem != nullptr )
        {
            // hover out
            manager()->closeTooltip();
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
                manager()->closeTooltip();
                manager()->openTooltip( selectedItem->item->tooltipData(), evt.pos );
            }
        }
        else
        {
            // Hover in
            manager()->openTooltip( selectedItem->item->tooltipData(), evt.pos );
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
    position -= m_tileOffset;
    Vector2i tileCoords = position / ( IconSize + PaddingThick );

    int idx = tileCoords.x() + m_tileBounds.x() * tileCoords.y();

    if ( idx < (int) m_items.size() )
    {
        return &m_items[idx];
    }

    return nullptr;
}