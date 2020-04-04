#include <ui/EquippedItemsView.h>
#include <resource/ResourceManager.h>
#include <graphics/RenderInterface.h>
#include <utils/GlobalConfig.h>
#include <game/Level.h>
#include <ui/Manager.h>
#include <components/ActorComponent.h>
#include <actions/ActionDefs.h>

UI::EquippedItemsView::EquippedItemsView(Manager* manager, Element* parent)
: Element(manager, parent)
{
    m_emptySlot = ResourceManager::get().getSprite("ui-frames", "inventory-slot");
    m_emptySlot.setRenderLayer(RenderLayer::UI);

    // Event callbacks
    addEventCallback(UEventType::Click, [this](UEvent& evt){
        onClick( evt.mouseButtonEvent );
    });

    setupSlots();
}

void UI::EquippedItemsView::updateSelf(uint32_t ticks, InputInterface &iinter, RenderInterface &rInter)
{
    int innerOffsetW = (SlotIconSize - GlobalConfig::TileSizePx) / 2;
    Vector2i innerOffset = { innerOffsetW, innerOffsetW };

    for ( auto const& item : m_slotData )
    {
        if ( item.equippedItem )
        {
            rInter.addScreenItem( item.itemSprite.renderObject(item.pos + innerOffset + DEBUG_OFFSET) );
        }
        else
        {
            rInter.addScreenItem( item.emptySprite.renderObject(item.pos + innerOffset + DEBUG_OFFSET) );
        }
    }
}

void UI::EquippedItemsView::onSizeSelf()
{
    layoutSlots();
}

void UI::EquippedItemsView::onMoveSelf()
{
    layoutSlots();
}

void UI::EquippedItemsView::layoutSlots()
{
    auto begin = globalPosition() + contentOffset();

    int leftOffset = innerBounds().w() / 2 - 66;
    int topOffset = 8;

    begin += {leftOffset, topOffset};

    for ( auto& s : m_slotData )
    {
        s.pos = begin + s.offset;
    }
}

void UI::EquippedItemsView::onClick(UI::UMouseButtonEvent &evt)
{
    auto ptr = itemFromPosition( evt.pos );
    if ( ptr )
    {
        if ( ptr->equippedItem )
        {
            auto unequipAction = std::make_shared<UnequipItemAction>( manager()->level(), m_entity, ptr->type );
            unequipAction->doAction();
        }
    }
}

const UI::EquippedItemSlot *UI::EquippedItemsView::itemFromPosition(Vector2i position) const
{
    for ( auto const& slot : m_slotData )
    {
        int dX = position.x() - slot.pos.x() - DEBUG_OFFSET.x();
        int dY = position.y() - slot.pos.y() - DEBUG_OFFSET.y();

        if ( dX >= 0 && dY >= 0 && dX < SlotIconSize && dY < SlotIconSize )
        {
            return &slot;
        }
    }

    return nullptr;
}

void UI::EquippedItemsView::attachEntity(EntityRef entity)
{
    m_entity = entity;
}

void UI::EquippedItemsView::reimportItems()
{
    auto actorC = manager()->level()->getComponents<ActorComponent>( m_entity );
    Assert( !!actorC );

    for ( auto& slot : m_slotData )
    {
        if ( actorC->character.hasEquipped(slot.type) )
        {
            auto item = actorC->character.getEquipped(slot.type);
            auto sprite = ResourceManager::get().getSprite( item->getSprite() );
            sprite.setRenderLayer( RenderLayer::UI );
            slot.itemSprite = sprite;
            slot.equippedItem = item;
        }
        else
        {
            slot.itemSprite = Sprite();
            slot.equippedItem = ItemPtr();
        }
    }
}

void UI::EquippedItemsView::setupSlots()
{
    // Setup slot information
    static const std::string SpritesheetName = "game-icons";

    auto addSlot = [this](auto&& slotType, auto&& spriteName, auto&& offset){
        static const std::string SpritesheetName = "game-icons";

        m_slotData.push_back({
                slotType,
                ResourceManager::get().getSprite(SpritesheetName, spriteName),
                offset,
                Vector2i{0, 0}
        });

        m_slotData.back().emptySprite.setRenderLayer(RenderLayer::UI);
    };

    const int halfIconSize = SlotIconSize / 2;
    const int vSpacing = SlotIconSize + SlotIconVSpacing;
    const int hSpacing = SlotIconSize + SlotIconHSpacing;

    addSlot(EquipSlot::Head,      "cowled",          Vector2i{halfIconSize + hSpacing * 0, vSpacing * 0});
    addSlot(EquipSlot::Headband,  "headband-knot",   Vector2i{halfIconSize + hSpacing * 1, vSpacing * 0});
    addSlot(EquipSlot::Eyes,      "spectacles",      Vector2i{halfIconSize + hSpacing * 2, vSpacing * 0});

    addSlot(EquipSlot::Shoulders, "pauldrons",       Vector2i{halfIconSize + hSpacing * 0, vSpacing * 1});
    addSlot(EquipSlot::Neck,      "necklace",        Vector2i{halfIconSize + hSpacing * 1, vSpacing * 1});
    addSlot(EquipSlot::Face,      "helmet",          Vector2i{halfIconSize + hSpacing * 2, vSpacing * 1});

    addSlot(EquipSlot::Chest,     "shirt",           Vector2i{halfIconSize + hSpacing * 0, vSpacing * 2});
    addSlot(EquipSlot::Body,      "robe",            Vector2i{halfIconSize + hSpacing * 1, vSpacing * 2});
    addSlot(EquipSlot::Armour,    "mail-shirt",      Vector2i{halfIconSize + hSpacing * 2, vSpacing * 2});

    addSlot(EquipSlot::MainHand,  "sword-hilt",      Vector2i{hSpacing * 0, vSpacing * 3});
    addSlot(EquipSlot::Belt,      "belt",            Vector2i{hSpacing * 1, vSpacing * 3});
    addSlot(EquipSlot::Wrists,    "gloves",          Vector2i{hSpacing * 2, vSpacing * 3});
    addSlot(EquipSlot::OffHand,   "sword-hilt",      Vector2i{hSpacing * 3, vSpacing * 3});

    addSlot(EquipSlot::LeftRing,  "ring",            Vector2i{halfIconSize + hSpacing * 0, vSpacing * 4});
    addSlot(EquipSlot::Feet,      "boots",           Vector2i{halfIconSize + hSpacing * 1, vSpacing * 4});
    addSlot(EquipSlot::RightRing, "ring",            Vector2i{halfIconSize + hSpacing * 2, vSpacing * 4});
}

UI::EquippedItemSlot::EquippedItemSlot(EquipSlot type, const Sprite &emptySprite, const Vector2i &offset,
                                       const Vector2i &pos)
        : type(type), emptySprite(emptySprite), offset(offset), pos(pos)
{}

