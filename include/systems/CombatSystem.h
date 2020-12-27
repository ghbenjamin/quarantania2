#pragma once

#include <systems/System.h>
#include <game/GameEvent.h>
#include <game/GameEventDefs.h>

class Actor;

class CombatSystem : public System,
                     public GEventSub<GameEvents::CombatMeleeAttack>
{
public:
    explicit CombatSystem(Level* parent);
    ~CombatSystem() override = default;

    void accept(GameEvents::CombatMeleeAttack *evt) override;

private:

    void acceptDamage( Damage const& dmg, EntityRef ref );

};