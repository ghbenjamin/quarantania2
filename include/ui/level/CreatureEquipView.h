#pragma once

#include <optional>
#include <ui/lib/Element.h>
#include <game/Defines.h>
#include <engine/Entity.h>

class Item;

namespace UI
{

struct EquipSlotView
{
    RectI offset;
    std::shared_ptr<Item> item;
    Sprite defaultSprite;
    std::optional<Sprite> itemSprite;
};


class EquipView : public Element
{
public:
    
    EquipView(Manager* manager, Element* parent);
    ~EquipView() override = default;

    void refresh( EntityRef entity );

protected:
    void updateSelf(uint32_t ticks, InputInterface &iinter, RenderInterface &rInter) override;

private:
    void addRegion( CreatureEquipSlot slot, SpritesheetKey const& key, Vector2i const& offset );
    void onMouseMove(UMouseMoveEvent const& evt);
    void onClick(UMouseButtonEvent const& evt);

    std::optional<CreatureEquipSlot> slotFromScreenPos(Vector2i pos) const;
    
private:
    std::unordered_map<CreatureEquipSlot, EquipSlotView> m_regions;
};



class EquipViewContainer : public Element
{
public:
    
    EquipViewContainer(Manager* manager, Element* parent);
    ~EquipViewContainer() override = default;

private:

};

}