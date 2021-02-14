#pragma once

#include <game/Defines.h>

namespace EnumParse
{
    DnDAlignment alignment( std::string const& input );
    ItemEquipSlot equipSlot(std::string const& input );
    ArmourType armourType( std::string const& input );
    CreatureSize creatureType( std::string const& input );
    ElementalDamageType elementalDamageType( std::string const& input );
    Skill skill( std::string const& input );
}


DiceRoll parseDiceRoll( std::string const& input );
