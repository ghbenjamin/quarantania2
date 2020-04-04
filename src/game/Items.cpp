#include <game/Items.h>
#include <utils/Json.h>
#include <utils/Logging.h>

//
//ItemData const* ItemManager::getItemData(std::string const &name) const
//{
//    return &m_itemData.at(name);
////}
//
//EquipSlotMask ItemManager::equipSlotsFromStr(std::vector<std::string_view> const& tokens)
//{
//    std::uint32_t mask = 0;
//
//    for ( auto const& tok : tokens )
//    {
//        if ( tok == "Head" )
//        {
//            mask |= (std::uint32_t) EquipSlot::Head;
//        }
//        else if ( tok == "Headband" )
//        {
//            mask |= (std::uint32_t) EquipSlot::Headband;
//        }
//        else if ( tok == "Eyes" )
//        {
//            mask |= (std::uint32_t) EquipSlot::Eyes;
//        }
//        else if ( tok == "Shoulders" )
//        {
//            mask |= (std::uint32_t) EquipSlot::Shoulders;
//        }
//        else if ( tok == "Neck" )
//        {
//            mask |= (std::uint32_t) EquipSlot::Neck;
//        }
//        else if ( tok == "Face" )
//        {
//            mask |= (std::uint32_t) EquipSlot::Face;
//        }
//        else if ( tok == "Chest" )
//        {
//            mask |= (std::uint32_t) EquipSlot::Chest;
//        }
//        else if ( tok == "Body" )
//        {
//            mask |= (std::uint32_t) EquipSlot::Body;
//        }
//        else if ( tok == "Armour" )
//        {
//            mask |= (std::uint32_t) EquipSlot::Armour;
//        }
//        else if ( tok == "Belt" )
//        {
//            mask |= (std::uint32_t) EquipSlot::Belt;
//        }
//        else if ( tok == "Wrists" )
//        {
//            mask |= (std::uint32_t) EquipSlot::Wrists;
//        }
//        else if ( tok == "Feet" )
//        {
//            mask |= (std::uint32_t) EquipSlot::Feet;
//        }
//        else if ( tok == "MainHand" )
//        {
//            mask |= (std::uint32_t) EquipSlot::MainHand;
//        }
//        else if ( tok == "OffHand" )
//        {
//            mask |= (std::uint32_t) EquipSlot::OffHand;
//        }
//        else if ( tok == "LeftRing" )
//        {
//            mask |= (std::uint32_t) EquipSlot::LeftRing;
//        }
//        else if ( tok == "RightRing" )
//        {
//            mask |= (std::uint32_t) EquipSlot::RightRing;
//        }
//    }
//
//    return mask;
//}

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

Item::Item(const std::string &name, ItemType type, int baseValue, int weight, const SpritesheetKey &sprite,
           const EquipSlotMask &equipSlots)
        : m_name(name), m_type(type), m_baseValue(baseValue), m_weight(weight), m_sprite(sprite), m_equipSlots(equipSlots)
{
}

const std::string &Item::getName() const
{
    return m_name;
}

ItemType Item::getType() const
{
    return m_type;
}

int Item::getBaseValue() const
{
    return m_baseValue;
}

int Item::getWeight() const
{
    return m_weight;
}

const SpritesheetKey &Item::getSprite() const
{
    return m_sprite;
}

const EquipSlotMask &Item::getEquipSlots() const
{
    return m_equipSlots;
}
