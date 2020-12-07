#pragma once

#include <systems/System.h>
#include <game/GameEvent.h>
#include <game/GameEventDefs.h>


class CombatSystem : public System,
                     public GEventSub<GameEvents::MeleeAttack>
{
public:
    explicit CombatSystem(Level* parent);
    ~CombatSystem() override = default;

    void accept(GameEvents::MeleeAttack *evt) override;
};