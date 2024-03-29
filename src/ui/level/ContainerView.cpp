#include <ui/level/ContainerView.h>
#include <ui/lib/Manager.h>
#include <graphics/RenderInterface.h>
#include <components/ContainerComponent.h>
#include <resource/ResourceManager.h>
#include <game/Level.h>
#include <utils/GlobalConfig.h>
#include <components/ActorComponent.h>
#include <utils/Memory.h>

using namespace UI;

ContainerView::ContainerView(Manager* manager, Element* parent, Level* level, Vector2i iconDims)
: Element(manager, parent), m_iconDims(iconDims), m_level(level)
{
    auto emptySprite = ResourceManager::get().getSprite("game_ui/empty-item-slot");

    setLayout<GridLayout>( m_iconDims, emptySprite.size(), 2 );
    
    auto const& patch = ResourceManager::get().getNinePatch( "simple-border" ).getPatch();
    setBackground( patch );
    setBorderWidth( patch.getBorderWidth() );
    
    for ( int i = 0; i < m_iconDims.area(); i++ )
    {
        auto item = manager->createElement<ContainerViewItem>( this, level, m_entity, emptySprite );
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
    
    auto containerC = m_level->ecs().getComponents<ContainerComponent>(entity);
    
    for ( int i = 0; i < (int) containerC->items.size() && i < (int) m_items.size(); i++ )
    {
        m_items[i]->setItem( containerC->items[i] );
    }
}

ContainerViewItem::ContainerViewItem( Manager *manager, Element *parent, Level* level, EntityRef entity, Sprite const& backgroundSprite )
    : Element(manager, parent), m_entity(entity), m_level(level)
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
        auto actorC = m_level->ecs().getComponents<ActorComponent>(m_entity);
        auto equipSlot = actorC->actor.canEquipItem(m_item);
    
        if (equipSlot)
        {
            m_level->events().broadcast<GameEvents::ItemEquip>( m_entity, m_item, *equipSlot );
        }
    }
}

std::shared_ptr<Tooltip> ContainerViewItem::tooltipSpawner()
{
    if (m_item)
    {
        return manager()->createElement<SimpleTooltip>( nullptr, m_item->getName(), m_item->getDescription() );
    }
    else
    {
        return {};
    }
}
