#pragma once

#include <engine/Component.h>
#include <utils/Colour.h>


struct LightingComponent : public Component<LightingComponent>
{
    LightingComponent( float intensity, Colour colour );
    ~LightingComponent() override = default;
    
    float intensity;
    Colour colour;
};
