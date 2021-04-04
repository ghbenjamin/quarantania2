#include <ui/level/ContainerView.h>
#include <ui/lib/Manager.h>
#include <graphics/RenderInterface.h>
#include <components/ContainerComponent.h>
#include <resource/ResourceManager.h>
#include <game/Level.h>
#include <utils/GlobalConfig.h>
#include <components/ActorComponent.h>

using namespace UI;

ContainerView::ContainerView(Manager* manager, Element* parent, Vector2i iconDims)
: Element(manager, parent), m_iconDims(iconDims)
{
    auto emptySprite = ResourceManager::get().getSprite("game_ui/empty-item-slot");
    emptySprite.setRenderLayer(RenderLayer::UI);

    setLayout<GridLayout>( m_iconDims, emptySprite.size(), 2 );
    
    auto const& patch = ResourceManager::get().getNinePatch( "simple-border" ).getPatch();
    setBackground( patch );
    setBorderWidth( patch.getBorderWidth() );
    
    for ( int i = 0; i < m_iconDims.area(); i++ )
    {
        auto item = manager->createElement<ContainerViewItem>( this, m_entity, emptySprite );
        m_items.push_back( item );
    }
}

void ContainerView::refresh(EntityRef entity)
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
    
    for ( int i = 0; i < (int) containerC->items.size() && i < (int) m_items.size(); i++ )
    {
        m_items[i]->setItem( containerC->items[i] );
    }
}

ContainerViewItem::ContainerViewItem( Manager *manager, Element *parent, EntityRef entity, Sprite const& backgroundSprite )
    : Element(manager, parent), m_entity(entity)
{
    setPreferredContentSize( backgroundSprite.size() );
    setBackground( backgroundSprite );
    setLayout<CenterLayout>();
    
    m_icon = manager->createElement<Icon>( this );

    addEventCallback(UEventType::Click, [this](UEvent& evt){ this->onClick(); });
    setTooltipSpawner( [this](){ return this->tooltipSpawner(); } );
}

void ContainerViewItem::setItem( std::shared_ptr<Item> item )
{
    m_item = item;
    m_icon->setSprite( item->getSprite() );
}

void ContainerViewItem::reset()
{
    m_item.reset();
    m_icon->clearSprite();
}

void ContainerViewItem::setEntity(EntityRef entity)
{
    m_entity = entity;
}

void ContainerViewItem::onClick()
{
    if (m_item && m_entity != EntityNull)
    {
        auto actorC = manager()->level()->ecs().getComponents<ActorComponent>(m_entity);
        auto equipSlot = actorC->actor.canEquipItem(m_item);
    
        if (equipSlot)
        {
            manager()->level()->events().broadcast<GameEvents::ItemEquip>( m_entity, m_item, *equipSlot );
        }
    }
}

std::optional<TooltipData> ContainerViewItem::tooltipSpawner()
{
    if (m_item)
    {
        return TooltipData {
            m_item->getName(),
            "Item",
            m_item->getDescription()
        };
    }
    else
    {
        return {};
    }
}
