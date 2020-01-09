#pragma once

#include <systems/System.h>
#include <game/GEvent.h>
#include <game/GEventDefs.h>

namespace Systems
{

class Combat : public System,
               public GEventSub<GEvents::MeleeAttack>
{
public:
    explicit Combat(Level* parent);
    ~Combat() override = default;

    void accept(GEvents::MeleeAttack *evt) override;
};

}