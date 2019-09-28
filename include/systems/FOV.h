#pragma once

#include <systems/System.h>
#include <game/GEvent.h>
#include <game/GEventDefs.h>
#include <resource/Sprite.h>

namespace Systems
{

class FOV : public System,
            public GEventSub<GEvents::EntityMove>
{
public:
    explicit FOV(Level *parent);
    ~FOV() override = default;

    void accept(GEvents::EntityMove *evt) override;

private:

    // This seems like a logical place to keep these?

    Sprite m_fovHidden;
    Sprite m_fovFog;
};

}