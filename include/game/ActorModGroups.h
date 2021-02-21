#pragma once

#include <game/ActorModifier.h>

class ActorModFactory
{
public:

    // Feats
    ActorModGroup featPowerAttack();

    // Statuses
    ActorModGroup statusExhausted(int roundRemove);
    ActorModGroup statusFatigued(int roundRemove);
    ActorModGroup statusSickened(int roundRemove);
};