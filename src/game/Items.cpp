#include <game/Items.h>
#include <utils/Json.h>
#include <utils/Logging.h>

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

        auto entryEquips = item.FindMember("EquipSlots");
        if ( entryEquips != item.MemberEnd() )
        {
            std::vector<std::string_view> slots;
            for ( auto const& sv : entryEquips->value.GetArray() )
            {
                slots.push_back( sv.GetString() );
            }

            data.equipSlots = EquipSlotMask( equipSlotsFromStr(slots) );
        }
        else
        {
            // If the weapon doesn't specify, assume that it can be wielded in either hand
            data.equipSlots = EquipSlotMask({ EquipSlot::MainHand, EquipSlot::OffHand });
        }

        m_itemData.emplace( data.name, std::move(data) );
    }
}

ItemData const* ItemManager::getItemData(std::string const &name) const
{
    return &m_itemData.at(name);
}

EquipSlotMask ItemManager::equipSlotsFromStr(std::vector<std::string_view> const& tokens)
{
    std::uint32_t mask = 0;

    for ( auto const& tok : tokens )
    {
        if ( tok == "Head" )
        {
            mask |= (std::uint32_t) EquipSlot::Head;
        }
        else if ( tok == "Headband" )
        {
            mask |= (std::uint32_t) EquipSlot::Headband;
        }
        else if ( tok == "Eyes" )
        {
            mask |= (std::uint32_t) EquipSlot::Eyes;
        }
        else if ( tok == "Shoulders" )
        {
            mask |= (std::uint32_t) EquipSlot::Shoulders;
        }
        else if ( tok == "Neck" )
        {
            mask |= (std::uint32_t) EquipSlot::Neck;
        }
        else if ( tok == "Face" )
        {
            mask |= (std::uint32_t) EquipSlot::Face;
        }
        else if ( tok == "Chest" )
        {
            mask |= (std::uint32_t) EquipSlot::Chest;
        }
        else if ( tok == "Body" )
        {
            mask |= (std::uint32_t) EquipSlot::Body;
        }
        else if ( tok == "Armour" )
        {
            mask |= (std::uint32_t) EquipSlot::Armour;
        }
        else if ( tok == "Belt" )
        {
            mask |= (std::uint32_t) EquipSlot::Belt;
        }
        else if ( tok == "Wrists" )
        {
            mask |= (std::uint32_t) EquipSlot::Wrists;
        }
        else if ( tok == "Feet" )
        {
            mask |= (std::uint32_t) EquipSlot::Feet;
        }
        else if ( tok == "MainHand" )
        {
            mask |= (std::uint32_t) EquipSlot::MainHand;
        }
        else if ( tok == "OffHand" )
        {
            mask |= (std::uint32_t) EquipSlot::OffHand;
        }
        else if ( tok == "LeftRing" )
        {
            mask |= (std::uint32_t) EquipSlot::LeftRing;
        }
        else if ( tok == "RightRing" )
        {
            mask |= (std::uint32_t) EquipSlot::RightRing;
        }
    }

    return mask;
}


Item::Item(ItemData const *data)
: m_data(data)
{
}

ItemData const *Item::data() const
{
    return m_data;
}

EquipSlotMask::EquipSlotMask(int32_t mask)
    : m_mask(mask)
{}

EquipSlotMask::EquipSlotMask()
    : m_mask(0)
{}

EquipSlotMask::EquipSlotMask(std::initializer_list<EquipSlot> slots)
{
    m_mask = 0;
    for ( EquipSlot const& s : slots )
    {
        m_mask |= (std::uint32_t) s;
    }
}

std::int32_t EquipSlotMask::mask() const
{
    return m_mask;
}

std::set<EquipSlot> EquipSlotMask::unpack() const
{
    std::set<EquipSlot> out{};

    for ( int i = 0; i < 16; i++ )
    {
        std::uint32_t flag = (1u << i);
        if ( m_mask & flag )
        {
            out.insert( static_cast<EquipSlot>(flag) );
        }
    }

    return out;
}
