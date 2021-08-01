#include <ui/level/CreatureEquipView.h>

#include <ui/lib/Manager.h>
#include <game/Level.h>
#include <resource/ResourceManager.h>
#include <graphics/RenderInterface.h>
#include <components/ActorComponent.h>
#include <game/GameParse.h>
#include <utils/Memory.h>


using namespace UI;

EquipView::EquipView(Manager *manager, Element *parent, Level* level)
    : Element(manager, parent), m_currEntity(EntityNull), m_level(level)
{
    auto const& patch = ResourceManager::get().getNinePatch( "simple-border" ).getPatch();
    setBackground( patch );
    setBorderWidth( patch.getBorderWidth() );

    auto emptySlot = ResourceManager::get().getSprite("game_ui/empty-item-slot");

    setId("ui-equip-view");
    setLayout<GridLayout>( Vector2{ 3, 4 }, emptySlot.size(), 2 );
    
    addRegion(CreatureEquipSlot::Head, "game_ui/pointy-hat");
    addRegion(CreatureEquipSlot::Shoulders, "game_ui/pauldrons");
    addRegion(CreatureEquipSlot::Body, "game_ui/walking-boot");
    addRegion(CreatureEquipSlot::Arms, "game_ui/walking-boot");
    
    addRegion(CreatureEquipSlot::Belt, "game_ui/belt");
    addRegion(CreatureEquipSlot::Feet, "game_ui/leg-armor");
    addRegion(CreatureEquipSlot::Ring1, "game_ui/ring");
    addRegion(CreatureEquipSlot::Neck, "game_ui/pearl-necklace");
    
    addRegion(CreatureEquipSlot::Ring2, "game_ui/ring");
    addRegion(CreatureEquipSlot::LeftHand, "game_ui/pointy-sword");
    addRegion(CreatureEquipSlot::Armour, "game_ui/abdominal-armor");
    addRegion(CreatureEquipSlot::RightHand, "game_ui/pointy-sword");
    
    for ( auto &[k, v] : m_regions )
    {
        v->setBackground( emptySlot );
        v->setPreferredContentSize( emptySlot.size() );
    }
}

void EquipView::addRegion( CreatureEquipSlot slot, SpritesheetKey const &key )
{
    auto item = manager()->createElement<EquipViewItem>( this, m_level, m_currEntity, slot, key );
    m_regions.emplace( slot, item );
}

void EquipView::refresh(EntityRef entity)
{
    // Clear our existing item data
    for ( auto &[k, v] : m_regions )
    {
        v->resetItem();
        v->setEntity(entity);
    }
    
    m_currEntity = entity;

    if (entity == EntityNull)
    {
        return;
    }

    auto actorC = m_level->ecs().getComponents<ActorComponent>( entity );

    // Load the equipped items for the current entity
    for ( auto const& [k, v] : actorC->actor.getAllEquippedItems() )
    {
        auto sprite = ResourceManager::get().getSprite( v->getSprite() );
        m_regions.at(k)->setItem( v, sprite );
    }
}

EquipViewItem::EquipViewItem( Manager *manager, Element *parent, Level* level, EntityRef entity, CreatureEquipSlot slot,
                                  SpritesheetKey defaultSprite )
      : Element(manager, parent), m_slot(slot), m_defaultSprite( ResourceManager::get().getSprite( defaultSprite )),
        m_entity(entity), m_level(level)
{
    setLayout<CenterLayout>();
    setPreferredContentSize({32, 32});
    
    m_defaultSprite.setPermanentColour(Colour::White.withAlpha(170));
    
    m_icon = manager->createElement<Icon>( this, m_defaultSprite );
    m_icon->setPreferredContentSize( m_defaultSprite.size() );
    
    
    setTooltipSpawner( [this](){ return this->tooltipSpawner(); } );
    addEventCallback(UEventType::Click, [this](UEvent& evt){ this->onClick(); });
}

void EquipViewItem::setItem( std::shared_ptr<Item> item, Sprite const &itemSprite )
{
    m_item = item;
    m_icon->setSprite( itemSprite );
}

void EquipViewItem::resetItem()
{
    m_item.reset();
    m_icon->setSprite( m_defaultSprite );
}

std::shared_ptr<Tooltip> EquipViewItem::tooltipSpawner()
{
    if ( m_item )
    {
        return manager()->createElement<SimpleTooltip>( nullptr, m_item->getName(), m_item->getDescription() );
    }
    else
    {
        return manager()->createElement<SimpleTooltip>( nullptr, EnumToString::creatureEquipSlot( m_slot ), "");
    }
}

void EquipViewItem::onClick()
{
    if (m_item)
    {
        m_level->events().broadcast<GameEvents::ItemUnequip>( m_entity, m_slot );
    }
}

void EquipViewItem::setEntity(EntityRef entity)
{
    m_entity = entity;
}
