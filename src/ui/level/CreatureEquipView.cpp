#include <ui/level/CreatureEquipView.h>

#include <ui/lib/Manager.h>
#include <game/Level.h>
#include <resource/ResourceManager.h>
#include <graphics/RenderInterface.h>
#include <components/ActorComponent.h>
#include <game/GameParse.h>


using namespace UI;

EquipView::EquipView(Manager *manager, Element *parent)
    : Element(manager, parent), m_currEntity(EntityNull)
{
    auto const& patch = ResourceManager::get().getNinePatch( "simple-border" );
    NinePatch np = { patch.texture(), patch.offsets() };
    setBackground( np );
    setBorderWidth( patch.borderWidth() );


    auto emptySlot = ResourceManager::get().getSprite("game_ui/empty-item-slot");
    emptySlot.setRenderLayer(RenderLayer::UI);
    
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
    auto item = manager()->createElement<EquipViewItem>( this, m_currEntity, slot, key );
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

    auto actorC = manager()->level()->ecs().getComponents<ActorComponent>( entity );

    // Load the equipped items for the current entity
    for ( auto const& [k, v] : actorC->actor.getAllEquippedItems() )
    {
        auto sprite = ResourceManager::get().getSprite( v->getSprite() );
        sprite.setRenderLayer( RenderLayer::UI );
        
        m_regions.at(k)->setItem( v, sprite );
    }
}

EquipViewItem::EquipViewItem( Manager *manager, Element *parent, EntityRef entity, CreatureEquipSlot slot,
                                  SpritesheetKey defaultSprite )
      : Element(manager, parent), m_slot(slot), m_defaultSprite( ResourceManager::get().getSprite( defaultSprite )),
        m_entity(entity)
{
    setLayout<CenterLayout>();
    setPreferredContentSize({32, 32});
    
    m_defaultSprite.setRenderLayer( RenderLayer::UI );
    m_defaultSprite.setColour( Colour::White.withAlpha(170) );
    
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

std::optional<TooltipData> EquipViewItem::tooltipSpawner()
{
    if ( m_item )
    {
        return TooltipData {
            m_item->getName(),
            EnumToString::creatureEquipSlot( m_slot ),
            m_item->getDescription()
        };
    }
    else
    {
        return TooltipData {
            EnumToString::creatureEquipSlot( m_slot ),
        };
    }
}

void EquipViewItem::onClick()
{
    if (m_item)
    {
        manager()->level()->events().broadcast<GameEvents::ItemUnequip>( m_entity, m_slot );
    }
}

void EquipViewItem::setEntity(EntityRef entity)
{
    m_entity = entity;
}
