#pragma once

#include <game/ActorModifier.h>

class ActorModFactory
{
public:

    // API
    static ActorModGroup fromId( std::string const& id, int duration );
    static ActorModGroup fromId( std::string const& id );


private:

    // Debug
    static ActorModGroup featDebug();

    // Feats
    static ActorModGroup featPowerAttack();

    // Statuses
    static ActorModGroup statusExhausted(int roundRemove);
    static ActorModGroup statusFatigued(int roundRemove);
    static ActorModGroup statusSickened(int roundRemove);
};