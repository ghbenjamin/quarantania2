#include <game/Items.h>
#include <utils/Json.h>

void ItemManager::loadAllData()
{
// MSVC compiler error
#ifdef GetObject
#undef GetObject
#endif

    rapidjson::Document doc = JsonUtils::loadFromPath( "../resource/data/items.json" );

    // Parse weapons

    for ( auto const& item : doc.GetObject().FindMember("weapons")->value.GetArray() )
    {
        ItemData data{};
        data.type = ItemType::Weapon;

        auto entryName = item.FindMember("Name");
        if ( entryName != item.MemberEnd() )
        {
            data.name = entryName->value.GetString();
        }

        auto entryWeight = item.FindMember("Weight");
        if ( entryWeight != item.MemberEnd() )
        {
            data.weight = std::atoi( entryWeight->value.GetString() );
        }

        auto entryValue = item.FindMember("Value");
        if ( entryValue != item.MemberEnd() )
        {
            data.baseValue = std::atoi( entryValue->value.GetString() );
        }

        auto entrySprite = item.FindMember("Sprite");
        if ( entrySprite != item.MemberEnd() )
        {
            data.sprite = SpritesheetKey{ entrySprite->value.GetArray()[0].GetString(),
                                          entrySprite->value.GetArray()[1].GetString() };
        }

        m_itemData.emplace( data.name, std::move(data) );
    }
}

ItemData const* ItemManager::getItemData(std::string const &name) const
{
    return &m_itemData.at(name);
}

Item::Item(ItemData const *data)
: m_data(data)
{
}

ItemData const *Item::data() const
{
    return m_data;
}
