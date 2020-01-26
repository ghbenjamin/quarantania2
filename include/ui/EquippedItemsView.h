#pragma once

#include <ui/Element.h>
#include <game/Entity.h>

#include <vector>

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
    EquippedItemSlotType type;
    bool visible = true;
    Sprite sprite;
    Vector2i offset;
    Vector2i pos;
};


// View into the items an actor has equipped - normally the player
class EquippedItemsView : public Element
{

public:
    EquippedItemsView();
    ~EquippedItemsView() override = default;

private:

    void updateSelf(uint32_t ticks, InputInterface &iinter, RenderInterface &rInter) override;
    void onSizeSelf() override;
    void onMoveSelf() override;

    void layoutSlots();

    std::vector<EquippedItemSlot> m_slotData;
};

}