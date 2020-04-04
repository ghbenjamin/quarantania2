#pragma once

#include <memory>
#include <unordered_map>
#include <set>

#include <resource/Spritesheet.h>


enum class ItemType
{
    None,
    Weapon,
    Apparel
};

enum class EquipSlot
{
    Head        = (1u << 0),
    Headband    = (1u << 1),
    Eyes        = (1u << 2),
    Shoulders   = (1u << 3),
    Neck        = (1u << 4),
    Face        = (1u << 5),
    Chest       = (1u << 6),
    Body        = (1u << 7),
    Armour      = (1u << 8),
    Belt        = (1u << 9),
    Wrists      = (1u << 10),
    Feet        = (1u << 11),
    MainHand    = (1u << 12),
    OffHand     = (1u << 13),
    LeftRing    = (1u << 14),
    RightRing   = (1u << 15),
};

class EquipSlotMask
{
public:
    EquipSlotMask();
    EquipSlotMask(int32_t mask);
    EquipSlotMask(std::initializer_list<EquipSlot> slots);

    ~EquipSlotMask() = default;

    std::int32_t mask( ) const;
    std::set<EquipSlot> unpack( ) const;

private:
    std::int32_t m_mask;
};


class Item
{
public:
    Item(const std::string &name, ItemType type, int baseValue, int weight, const SpritesheetKey &sprite,
         const EquipSlotMask &equipSlots);
    virtual ~Item() = default;

    const std::string &getName() const;
    ItemType getType() const;
    int getBaseValue() const;
    int getWeight() const;
    const SpritesheetKey &getSprite() const;
    const EquipSlotMask &getEquipSlots() const;

private:
    std::string m_name;
    ItemType m_type = ItemType::None;
    int m_baseValue;
    int m_weight;
    SpritesheetKey m_sprite;
    EquipSlotMask m_equipSlots;
};




using ItemPtr = std::shared_ptr<Item>;

//
//class ItemManager
//{
//public:
//    ItemManager() = default;
//    ~ItemManager() = default;
//
//    void loadAllData();
//    ItemPtr getItemData( std::string const& name) const;
//
//private:
//
//    static EquipSlotMask equipSlotsFromStr( std::vector<std::string_view> const& tokens );
//
//private:
//    std::unordered_map<std::string, const ItemData> m_itemData;
//};;