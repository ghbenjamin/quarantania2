#pragma once

#include <engine/Component.h>
#include <resource/Sprite.h>

struct RenderComponent : public Component<RenderComponent>
{
    explicit RenderComponent(Sprite const& s);
    ~RenderComponent() override = default;
    
    Sprite sprite;
};
