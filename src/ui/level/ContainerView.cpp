#include <ui/level/ContainerView.h>
#include <ui/lib/Manager.h>
#include <graphics/Primatives.h>
#include <graphics/RenderInterface.h>
#include <components/ContainerComponent.h>
#include <resource/ResourceManager.h>
#include <game/Level.h>
#include <utils/GlobalConfig.h>

UI::ContainerView::ContainerView(Manager* manager, Element* parent, Vector2i iconDims)
: Element(manager, parent), m_iconDims(iconDims), m_iconSpacing(2)
{
    setLayout<FreeLayout>();

    m_emptySlot = ResourceManager::get().getSprite("game_ui/empty-item-slot");
    m_emptySlot.setRenderLayer(RenderLayer::UI);

    auto [slotW, slotH] = m_emptySlot.size();
    setPreferredContentSize({
        slotW * m_iconDims.x() + m_iconSpacing * ( m_iconDims.x() - 1 ),
        slotH * m_iconDims.y() + m_iconSpacing * ( m_iconDims.y() - 1 )
    });
    
    slotW += m_iconSpacing;
    slotH += m_iconSpacing;
    
    // Render each of our positions - both the empty slot background and any items if present
    for (int j = 0; j < m_iconDims.y(); j++)
    {
        for (int i = 0; i < m_iconDims.x(); i++)
        {
            Vector2i offset = { i * slotW, j * slotH };
            auto item = manager->createElement<ContainerViewItem>( this, m_entity, m_emptySlot );
            item->setLocalPosition( offset );
            
            m_items.push_back(item);
        }
    }
    
}

void UI::ContainerView::refresh(EntityRef entity)
{
    m_entity = entity;
    
    for ( auto& item : m_items )
    {
        item->reset();
        item->setEntity(entity);
    }
    
    if (entity == EntityNull)
    {
        return;
    }
    
    auto containerC = manager()->level()->ecs().getComponents<ContainerComponent>(entity);
    
    for ( int i = 0; i < containerC->items.size() && i < m_items.size(); i++ )
    {
        m_items[i]->setItem( containerC->items[i] );
    }
}

UI::ContainerViewItem::ContainerViewItem( UI::Manager *manager, UI::Element *parent, EntityRef entity, Sprite const& backgroundSprite )
    : Element(manager, parent), m_entity(entity)
{
    setPreferredContentSize({32, 32});
    setBackground( backgroundSprite );
    
    m_icon = manager->createElement<Icon>( this );
    m_icon->setPreferredContentSize({32, 32});
}

void UI::ContainerViewItem::setItem( std::shared_ptr<Item> item )
{
    m_item = item;
    m_icon->setSprite( item->getSprite() );
}

void UI::ContainerViewItem::reset()
{
    m_item.reset();
    m_icon->clearSprite();
}

void UI::ContainerViewItem::setEntity(EntityRef entity)
{
    m_entity = entity;
}
