#pragma once

#include <systems/System.h>
#include <game/GEvent.h>
#include <game/GEventDefs.h>

class CombatSystem : public System,
                     public GEventSub<GEvents::MeleeAttack>
{
public:
    explicit CombatSystem(Level* parent);
    ~CombatSystem() override = default;

    void accept(GEvents::MeleeAttack *evt) override;
};