#pragma once

#include <systems/System.h>
#include <game/GEvent.h>
#include <game/GEventDefs.h>

namespace Systems
{

class Actors : public System
{
public:
    explicit Actors(Level* parent);
    ~Actors() override = default;

    void update(uint32_t ticks, RenderInterface &rInter) override;
};

}