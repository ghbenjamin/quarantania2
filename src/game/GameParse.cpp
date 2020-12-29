#include <game/GameParse.h>

#include <regex>

#include <utils/Assert.h>
#include <fmt/format.h>

DnDAlignment EnumParse::alignment(std::string const& input)
{
    if ( input == "LG" )
    {
        return DnDAlignment::LG;
    }
    else if ( input == "NG" )
    {
        return DnDAlignment::NG;
    }
    else if ( input == "CG" )
    {
        return DnDAlignment::CG;
    }
    else if ( input == "LN" )
    {
        return DnDAlignment::LN;
    }
    else if ( input == "N" )
    {
        return DnDAlignment::TN;
    }
    else if ( input == "CN" )
    {
        return DnDAlignment::CN;
    }
    else if ( input == "LE" )
    {
        return DnDAlignment::LE;
    }
    else if ( input == "NE" )
    {
        return DnDAlignment::NE;
    }
    else if ( input == "CE" )
    {
        return DnDAlignment::CE;
    }
    else
    {
        AssertAlwaysMsg( fmt::format( "Unexpected alignment: '{}'", input ) );
        return DnDAlignment::TN;
    }
}

EquipSlot EnumParse::equipSlot(std::string const& input)
{
    if ( input == "armor" )
    {
        return EquipSlot::Armor;
    }
    else if ( input == "arms" )
    {
        return EquipSlot::Arms;
    }
    else if ( input == "belt" )
    {
        return EquipSlot::Belt;
    }
    else if ( input == "body" )
    {
        return EquipSlot::Body;
    }
    else if ( input == "chest" )
    {
        return EquipSlot::Chest;
    }
    else if ( input == "eyes" )
    {
        return EquipSlot::Eyes;
    }
    else if ( input == "feet" )
    {
        return EquipSlot::Feet;
    }
    else if ( input == "hands" )
    {
        return EquipSlot::Hands;
    }
    else if ( input == "head" )
    {
        return EquipSlot::Head;
    }
    else if ( input == "headband" )
    {
        return EquipSlot::Headband;
    }
    else if ( input == "neck" )
    {
        return EquipSlot::Neck;
    }
    else if ( input == "ring" )
    {
        return EquipSlot::Ring;
    }
    else if ( input == "shield" )
    {
        return EquipSlot::Shield;
    }
    else if ( input == "shoulders" )
    {
        return EquipSlot::Shoulders;
    }
    else if ( input == "wrists" )
    {
        return EquipSlot::Wrists;
    }
    else if ( input == "weapon" )
    {
        return EquipSlot::Weapon;
    }
    else
    {
        return EquipSlot::None;
    }
}

ArmourType EnumParse::armourType(std::string const& input)
{
    if ( input == "Light armor" )
    {
        return ArmourType::Light;
    }
    else if ( input == "Medium armor" )
    {
        return ArmourType::Medium;
    }
    else if ( input == "Heavy armor" )
    {
        return ArmourType::Heavy;
    }
    if ( input == "Shields" )
    {
        return ArmourType::Shield;
    }

    AssertAlwaysMsg( fmt::format( "Unexpected inpit: '{}'", input ) );
    return ArmourType::Light;
}

CreatureSize EnumParse::creatureType(std::string const& input)
{
    if ( input == "Tiny" )
    {
        return CreatureSize::Tiny;
    }
    else if ( input == "Small" )
    {
        return CreatureSize::Small;
    }
    else if ( input == "Medium" )
    {
        return CreatureSize::Medium;
    }
    else if ( input == "Large" )
    {
        return CreatureSize::Large;
    }
    else if ( input == "Huge" )
    {
        return CreatureSize::Huge;
    }
    else if ( input == "Gargantuan" )
    {
        return CreatureSize::Gargantuan;
    }
    else if ( input == "Colossal" )
    {
        return CreatureSize::Colossal;
    }

    AssertAlwaysMsg( fmt::format( "Unexpected inpit: '{}'", input ) );
    return CreatureSize::Tiny;
}


ElementalDamageType EnumParse::elementalDamageType( std::string const& input )
{
    if ( input == "fire" )
    {
        return ElementalDamageType::Fire;
    }
    else if ( input == "acid" )
    {
        return ElementalDamageType::Acid;
    }
    else if ( input == "cold" )
    {
        return ElementalDamageType::Cold;
    }
    else if ( input == "electricity" )
    {
        return ElementalDamageType::Electricity;
    }

    AssertAlwaysMsg( fmt::format( "Unexpected inpit: '{}'", input ) );
    return ElementalDamageType::Fire;
}



DiceRoll parseDiceRoll(const std::string &input)
{
    std::regex r( R"(^(\d+)d(\d+)(?:([+-])(\d+))?$)" );
    std::smatch m;

    std::regex_match(input, m, r);

    if ( m[4].matched )
    {
        int dcount = std::stoi( m[1] );
        int dsize = std::stoi( m[2] );
        int dmod = std::stoi( m[4] );

        if ( m[3] == "-" )
        {
            dmod *= -1;
        }

        return DiceRoll{ dcount, dsize, dmod };
    }
    else if ( m[1].matched )
    {
        int dcount = std::stoi( m[1] );
        int dsize = std::stoi( m[2] );
        return DiceRoll{ dcount, dsize };
    }
    else
    {
        AssertAlwaysMsg( fmt::format("Unexpected dice roll: '{}'", input ) );
        return DiceRoll{0, 0};
    }
}
