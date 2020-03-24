#pragma once

#include <memory>
#include <unordered_map>

#include <resource/Spritesheet.h>
#include <set>


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


struct ItemData
{
    std::string name;
    ItemType type = ItemType::None;
    int baseValue = 0;
    int weight = 0;
    SpritesheetKey sprite;
    EquipSlotMask equipSlots;
};

class Item
{
public:
    Item(ItemData const* data);
    virtual ~Item() = default;

    ItemData const* data() const;

private:
    ItemData const* const m_data;
};




using ItemPtr = std::shared_ptr<Item>;


class ItemManager
{
public:
    ItemManager() = default;
    ~ItemManager() = default;

    void loadAllData();
    const ItemData * getItemData( std::string const& name) const;

private:

    static EquipSlotMask equipSlotsFromStr( std::vector<std::string_view> const& tokens );

private:
    std::unordered_map<std::string, const ItemData> m_itemData;
};;