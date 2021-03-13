#include <ui/level/CreatureEquipView.h>

#include <ui/lib/Manager.h>
#include <game/Level.h>
#include <game/Action.h>
#include <resource/ResourceManager.h>
#include <graphics/RenderInterface.h>
#include <components/ActorComponent.h>
#include <game/GameParse.h>


UI::EquipViewContainer::EquipViewContainer(UI::Manager *manager, UI::Element *parent)
    : Element(manager, parent)
{
    auto const& patch = ResourceManager::get().getNinePatch( "simple-border" );
    NinePatch np = { patch.texture(), patch.offsets() };
    
    setBackground( np );
    setBorderWidth( patch.borderWidth() );
    setId("ui-equip-container");
    
    const Vector2i ChildSize = {164, 205};
    const Vector2i ParentSize = ChildSize + Vector2i {10, 10};
    
    setPreferredContentSize(ParentSize);
    
    setLayout<CenterLayout>();
    auto child = manager->createElement<UI::EquipView>(this);
    
    child->setPreferredContentSize(ChildSize);
}

UI::EquipView::EquipView(UI::Manager *manager, UI::Element *parent)
    : Element(manager, parent), m_currEntity(EntityNull)
{
    setId("ui-equip-inner");
    setLayout<FreeLayout>();
    
    auto sprite = ResourceManager::get().getImageAsSprite( "equip-only" );
    setBackground( sprite );
    
    addRegion( CreatureEquipSlot::Head, "game_ui/pointy-hat", {24, 3} );
    addRegion( CreatureEquipSlot::Headband, "game_ui/headband-knot", { 66,3 } );
    addRegion( CreatureEquipSlot::Eyes, "game_ui/steampunk-goggles", { 108,3 } );
    
    addRegion( CreatureEquipSlot::Shoulders, "game_ui/pauldrons", { 24,45 });
    addRegion( CreatureEquipSlot::Neck, "game_ui/pearl-necklace", { 66,45 } );
    addRegion( CreatureEquipSlot::Chest, "game_ui/chest-armor", { 108,45 } );
    
    addRegion( CreatureEquipSlot::Belt, "game_ui/belt", { 24,87 } );
    addRegion( CreatureEquipSlot::Wrists, "game_ui/walking-boot", { 66,87 } );
    addRegion( CreatureEquipSlot::Arms, "game_ui/walking-boot", { 108,87 } );
    
    addRegion( CreatureEquipSlot::LeftHand, "game_ui/pointy-sword", { 3,129 } );
    addRegion( CreatureEquipSlot::Body, "game_ui/walking-boot", { 45,129 } );
    addRegion( CreatureEquipSlot::Armour, "game_ui/abdominal-armor", { 87,129 } );
    addRegion( CreatureEquipSlot::RightHand, "game_ui/pointy-sword", { 129,129 } );
    
    addRegion( CreatureEquipSlot::Ring1, "game_ui/ring",  { 24,170 } );
    addRegion( CreatureEquipSlot::Feet, "game_ui/leg-armor", { 66,170 } );
    addRegion( CreatureEquipSlot::Ring2, "game_ui/ring", { 108,170 } );
}

void UI::EquipView::addRegion(CreatureEquipSlot slot, const SpritesheetKey &key, const Vector2i &offset)
{
    auto item = manager()->createElement<EquipViewItem>( this, m_currEntity, slot, key );
    item->setLocalPosition(offset);
    
    m_regions.emplace( slot, item );
}

void UI::EquipView::refresh(EntityRef entity)
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

UI::EquipViewItem::EquipViewItem( UI::Manager *manager, UI::Element *parent, EntityRef entity, CreatureEquipSlot slot,
                                  SpritesheetKey defaultSprite )
      : Element(manager, parent), m_slot(slot), m_defaultSprite( ResourceManager::get().getSprite( defaultSprite )),
        m_entity(entity)
{
    setLayout<CenterLayout>();
    setPreferredContentSize({32, 32});
    
    m_defaultSprite.setRenderLayer( RenderLayer::UI );
    m_defaultSprite.setColour( Colour::White.withAlpha(170) );
    
    m_icon = manager->createElement<Icon>( this, m_defaultSprite );
    
    setTooltipSpawner( [this](){ return this->tooltipSpawner(); } );
    addEventCallback(UEventType::Click, [this](UEvent& evt){ this->onClick(); });
}

void UI::EquipViewItem::setItem( std::shared_ptr<Item> item, Sprite const &itemSprite )
{
    m_item = item;
    m_icon->setSprite( itemSprite );
}

void UI::EquipViewItem::resetItem()
{
    m_item.reset();
    m_icon->setSprite( m_defaultSprite );
}

UI::TooltipData UI::EquipViewItem::tooltipSpawner()
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

void UI::EquipViewItem::onClick()
{
    if (m_item)
    {
//        auto actorC = manager()->level()->ecs().getComponents<ActorComponent>(m_entity);
        
        manager()->level()->events().broadcast<GameEvents::ItemUnequip>( m_entity, m_slot );
    }
}

void UI::EquipViewItem::setEntity(EntityRef entity)
{
    m_entity = entity;
}
