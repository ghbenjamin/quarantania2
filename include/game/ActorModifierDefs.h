#pragma once

#include <game/ActorModifier.h>

class SingleAttackRoll;


enum class ActorModType
{
    AttackRolls,
};


namespace ActorMods
{
    struct ModAttackRoll
    {
        virtual void modify( SingleAttackRoll& roll ) = 0;
    };
}


struct ActorMod
{
    ActorModType type;
    
};




class ActorModFactory
{
public:

    // Statuses
    ActorModGroup statusSickened(int roundRemove);
};