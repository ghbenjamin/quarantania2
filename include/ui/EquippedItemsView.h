#pragma once

#include <ui/Element.h>
#include <game/Entity.h>

#include <vector>
#include <game/Items.h>

namespace UI
{

enum class EquippedItemSlotType
{
    Head,
    Headband,
    Eyes,
    Shoulders,
    Neck,
    Face,
    Chest,
    Body,
    Armour,
    Belt,
    Wrists,
    Ring1,
    Ring2,
    Feet,
    MainHand,
    OffHand
};

struct EquippedItemSlot
{
    EquippedItemSlot(EquipSlot type, const Sprite &emptySprite, const Vector2i &offset, const Vector2i &pos);
    ~EquippedItemSlot() = default;

    EquipSlot type;
    Sprite emptySprite;
    Vector2i offset;
    Vector2i pos;

    ItemPtr equippedItem;
    Sprite itemSprite;
};


// View into the items an actor has equipped - normally the player
class EquippedItemsView : public Element
{

public:
    EquippedItemsView();
    ~EquippedItemsView() override = default;

    void attachEntity(EntityRef entity);
    void reimportItems();

private:

    void onClick( UMouseButtonEvent& evt );

    void updateSelf(uint32_t ticks, InputInterface &iinter, RenderInterface &rInter) override;
    void onSizeSelf() override;
    void onMoveSelf() override;

    void layoutSlots();

private:
    EquippedItemSlot const* itemFromPosition( Vector2i position ) const;

    std::vector<EquippedItemSlot> m_slotData;
    EntityRef m_entity;
};

}