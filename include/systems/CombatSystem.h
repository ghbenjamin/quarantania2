#pragma once

#include <systems/System.h>
#include <game/GameEvent.h>
#include <game/GameEventDefs.h>

class Actor;

class CombatSystem : public System, public GameEventSub<CombatSystem>
{
public:
    explicit CombatSystem(Level* parent);
    ~CombatSystem() override = default;

    void operator()(GameEvents::CombatMeleeAttack& evt);

    template <typename T>
    void operator()(T&& t)
    {}

private:

    void acceptDamage( Damage const& dmg, EntityRef ref );

};