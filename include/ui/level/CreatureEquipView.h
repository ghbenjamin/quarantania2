#pragma once

#include <optional>
#include <ui/lib/Element.h>
#include <game/Defines.h>
#include <engine/Entity.h>

class Item;

namespace UI
{

class Icon;


class EquipViewItem : public Element
{
public:
    EquipViewItem(Manager* manager, Element* parent, EntityRef entity, CreatureEquipSlot slot, SpritesheetKey defaultSprite );
    ~EquipViewItem() override = default;
    
    void setEntity(EntityRef entity);
    void setItem( std::shared_ptr<Item> item, Sprite const& itemSprite );
    void resetItem();

private:

    void onClick();


    TooltipData tooltipSpawner();
    
    std::shared_ptr<Icon> m_icon;
    CreatureEquipSlot m_slot;
    Sprite m_defaultSprite;
    std::shared_ptr<Item> m_item;
    EntityRef m_entity;
};



class EquipView : public Element
{
public:
    
    EquipView(Manager* manager, Element* parent);
    ~EquipView() override = default;

    void refresh( EntityRef entity );

private:
    void addRegion( CreatureEquipSlot slot, SpritesheetKey const& key, Vector2i const& offset );
    
    
    std::unordered_map<CreatureEquipSlot, std::shared_ptr<EquipViewItem>> m_regions;
    EntityRef m_currEntity;
};



class EquipViewContainer : public Element
{
public:
    EquipViewContainer(Manager* manager, Element* parent);
    ~EquipViewContainer() override = default;
};

}