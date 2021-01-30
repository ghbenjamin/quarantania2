#pragma once

#include <ui/lib/Element.h>
#include <game/Defines.h>

namespace UI
{


struct EquipSlotView
{
    Sprite sprite;
    RectI offset;
};


class EquipUiInner : public Element
{
public:
    
    EquipUiInner(Manager* manager, Element* parent);
    ~EquipUiInner() override = default;

protected:
    void updateSelf(uint32_t ticks, InputInterface &iinter, RenderInterface &rInter) override;

private:
    void addRegion( CreatureEquipSlot slot, SpritesheetKey const& key, Vector2i const& offset );
    void onMouseMove(UMouseMoveEvent const& evt);
    void onClick(UMouseButtonEvent const& evt);
    
    std::optional<CreatureEquipSlot> slotFromScreenPos(Vector2i pos) const;
    
    std::unordered_map<CreatureEquipSlot, EquipSlotView> m_regions;
};


class EquipUi : public Element
{
public:
    
    EquipUi(Manager* manager, Element* parent);
    ~EquipUi() override = default;

private:
};



}
