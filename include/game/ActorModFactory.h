#pragma once

#include <game/ActorModifier.h>

class ActorModFactory
{
public:

    static ActorModGroup fromId( std::string const& id, int duration );
    static ActorModGroup fromId( std::string const& id );



    // Feats
    static ActorModGroup featPowerAttack();

    // Statuses
    static ActorModGroup statusExhausted(int roundRemove);
    static ActorModGroup statusFatigued(int roundRemove);
    static ActorModGroup statusSickened(int roundRemove);
};