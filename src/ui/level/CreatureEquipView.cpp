#include <ui/level/CreatureEquipView.h>

#include <ui/lib/Manager.h>
#include <game/Level.h>
#include <game/Action.h>
#include <resource/ResourceManager.h>
#include <graphics/RenderInterface.h>


UI::EquipUi::EquipUi(UI::Manager *manager, UI::Element *parent) : Element(manager, parent)
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
    auto child = manager->createElement<UI::EquipUiInner>(this);
    
    child->setPreferredContentSize(ChildSize);
}

UI::EquipUiInner::EquipUiInner(UI::Manager *manager, UI::Element *parent) : Element(manager, parent)
{
    setId("ui-equip-inner");
    
    auto sprite = ResourceManager::get().getImageAsSprite( "equip-only" );
    setBackground( sprite );
    
    addRegion( CreatureEquipSlot::Head, "game_icons/pointy-hat", {24, 3} );
    addRegion( CreatureEquipSlot::Headband, "game_icons/headband-knot", { 66,3 } );
    addRegion( CreatureEquipSlot::Eyes, "game_icons/steampunk-goggles", { 108,3 } );
    
    addRegion( CreatureEquipSlot::Shoulders, "game_icons/pauldrons", { 24,45 });
    addRegion( CreatureEquipSlot::Neck, "game_icons/pearl-necklace", { 66,45 } );
    addRegion( CreatureEquipSlot::Chest, "game_icons/chest-armor", { 108,45 } );
    
    addRegion( CreatureEquipSlot::Belt, "game_icons/belt", { 24,87 } );
    addRegion( CreatureEquipSlot::Wrists, "game_icons/walking-boot", { 66,87 } );
    addRegion( CreatureEquipSlot::Arms, "game_icons/walking-boot", { 108,87 } );
    
    addRegion( CreatureEquipSlot::LeftHand, "game_icons/pointy-sword", { 3,129 } );
    addRegion( CreatureEquipSlot::Body, "game_icons/walking-boot", { 45,129 } );
    addRegion( CreatureEquipSlot::Armour, "game_icons/abdominal-armor", { 87,129 } );
    addRegion( CreatureEquipSlot::RightHand, "game_icons/pointy-sword", { 129,129 } );
    
    addRegion( CreatureEquipSlot::Ring1, "game_icons/ring",  { 24,170 } );
    addRegion( CreatureEquipSlot::Feet, "game_icons/leg-armor", { 66,170 } );
    addRegion( CreatureEquipSlot::Ring2, "game_icons/ring", { 108,170 } );
    
    
    addEventCallback(UEventType::MouseMove, [this] (UEvent& evt) {
        this->onMouseMove(evt.mouseMoveEvent);
    });
    
    addEventCallback(UEventType::Click, [this] (UEvent& evt) {
        this->onClick(evt.mouseButtonEvent);
    });
    
}

void UI::EquipUiInner::addRegion(CreatureEquipSlot slot, const SpritesheetKey &key, const Vector2i &offset)
{
    EquipSlotView view;
    view.sprite = ResourceManager::get().getSprite( key );
    view.offset = { offset, Vector2i{32, 32} };
    
    view.sprite.setRenderLayer( RenderLayer::UI );
    m_regions.emplace( slot, view );
}

void UI::EquipUiInner::updateSelf(uint32_t ticks, InputInterface &iinter, RenderInterface &rInter)
{
    auto offset = globalPosition();
    
    for (auto const& [k, v] : m_regions )
    {
        rInter.addScreenItem( v.sprite.renderObject( offset + v.offset.left() ) );
    }
}

void UI::EquipUiInner::onMouseMove(UMouseMoveEvent const& evt)
{

}

void UI::EquipUiInner::onClick(const UI::UMouseButtonEvent &evt)
{
    auto clicked = slotFromScreenPos( evt.pos );
    
    if (clicked)
    {
        if ( evt.button == UMouseButtonEvent::LeftMouseButton )
        {
        
        }
        else if ( evt.button == UMouseButtonEvent::RightMouseButton )
        {
        
        }
    }
}

std::optional<CreatureEquipSlot> UI::EquipUiInner::slotFromScreenPos(Vector2i pos) const
{
    auto localPos = pos - globalPosition() - contentOffset();
    
    for (auto const& [k, v] : m_regions )
    {
        if ( v.offset.contains(localPos) )
        {
            return k;
        }
    }
    
    return {};
}
