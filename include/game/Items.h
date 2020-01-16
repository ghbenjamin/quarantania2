#pragma once

#include <memory>
#include <unordered_map>

#include <resource/Spritesheet.h>


struct ItemData
{
    std::string name;
    int value;
    int weight;
    SpritesheetKey sprite;
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

    std::unordered_map<std::string, const ItemData> m_itemData;
};;