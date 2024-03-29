#pragma once

#include <optional>
#include <ui/lib/Element.h>

#include <game/Defines.h>
#include <engine/Entity.h>

class Item;
class Level;

namespace UI
{

class Icon;


class EquipViewItem : public Element
{
public:
    EquipViewItem(Manager* manager, Element* parent, EntityRef entity, Level* level, CreatureEquipSlot slot, SpritesheetKey defaultSprite );
    
    EquipViewItem( Manager *manager, Element *parent, Level *level, EntityRef entity, CreatureEquipSlot slot,
                   SpritesheetKey defaultSprite );
    
    ~EquipViewItem() override = default;
    
    void setEntity(EntityRef entity);
    void setItem( std::shared_ptr<Item> item, Sprite const& itemSprite );
    void resetItem();

private:

    void onClick();
    std::shared_ptr<Tooltip> tooltipSpawner();
    
    Level* m_level;
    std::shared_ptr<Icon> m_icon;
    CreatureEquipSlot m_slot;
    Sprite m_defaultSprite;
    std::shared_ptr<Item> m_item;
    EntityRef m_entity;
};



class EquipView : public Element
{
public:
    
    EquipView(Manager* manager, Element* parent, Level* level);
    ~EquipView() override = default;

    void refresh( EntityRef entity );

private:
    void addRegion( CreatureEquipSlot slot, SpritesheetKey const &key );
    
    Level* m_level;
    std::unordered_map<CreatureEquipSlot, std::shared_ptr<EquipViewItem>> m_regions;
    EntityRef m_currEntity;
};


}