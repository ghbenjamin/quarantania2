#include <ui/EquippedItemsView.h>
#include <resource/ResourceManager.h>
#include <graphics/RenderInterface.h>
#include <utils/GlobalConfig.h>
#include <game/Level.h>
#include <ui/Manager.h>
#include <components/ActorComponent.h>

UI::EquippedItemsView::EquippedItemsView()
{
    // Event callbacks
    addEventCallback(UEventType::Click, [this](UEvent& evt){
        onClick( evt.mouseButtonEvent );
    });

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

    addSlot(EquipSlot::Head,      "cowled",          Vector2i{16, 0});
    addSlot(EquipSlot::Headband,  "headband-knot",   Vector2i{52, 0});
    addSlot(EquipSlot::Eyes,      "spectacles",      Vector2i{88, 0});

    addSlot(EquipSlot::Shoulders, "pauldrons",       Vector2i{16, 36});
    addSlot(EquipSlot::Neck,      "necklace",        Vector2i{52, 36});
    addSlot(EquipSlot::Face,      "helmet",          Vector2i{88, 36});

    addSlot(EquipSlot::Chest,     "shirt",           Vector2i{16, 72});
    addSlot(EquipSlot::Body,      "robe",            Vector2i{52, 72});
    addSlot(EquipSlot::Armour,    "mail-shirt",      Vector2i{88, 72});

    addSlot(EquipSlot::MainHand,  "sword-hilt",      Vector2i{0, 108});
    addSlot(EquipSlot::Belt,      "belt",            Vector2i{36, 108});
    addSlot(EquipSlot::Wrists,    "gloves",          Vector2i{72, 108});
    addSlot(EquipSlot::OffHand,   "sword-hilt",      Vector2i{108, 108});

    addSlot(EquipSlot::LeftRing,  "ring",            Vector2i{16, 144});
    addSlot(EquipSlot::Feet,      "boots",           Vector2i{52, 144});
    addSlot(EquipSlot::RightRing, "ring",            Vector2i{88, 144});
}

void UI::EquippedItemsView::updateSelf(uint32_t ticks, InputInterface &iinter, RenderInterface &rInter)
{
    for ( auto const& item : m_slotData )
    {
        if ( item.equippedItem )
        {
            rInter.addScreenItem( item.itemSprite.renderObject(item.pos) );
        }
        else
        {
            rInter.addScreenItem( item.emptySprite.renderObject(item.pos) );
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
        Logging::log( (int)ptr->type );
    }
}

const UI::EquippedItemSlot *UI::EquippedItemsView::itemFromPosition(Vector2i position) const
{
    for ( auto const& slot : m_slotData )
    {
        int dX = position.x() - slot.pos.x();
        int dY = position.y() - slot.pos.y();

        if ( dX >= 0 && dY >= 0 && dX < GlobalConfig::TileSizePx && dY < GlobalConfig::TileSizePx )
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
            auto sprite = ResourceManager::get().getSprite( item->data()->sprite );
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

UI::EquippedItemSlot::EquippedItemSlot(EquipSlot type, const Sprite &emptySprite, const Vector2i &offset,
                                       const Vector2i &pos)
        : type(type), emptySprite(emptySprite), offset(offset), pos(pos)
{}

