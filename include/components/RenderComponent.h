#pragma once

#include <engine/Component.h>
#include <resource/Sprite.h>

struct RenderComponent : public Component<RenderComponent>
{
    explicit RenderComponent(Sprite const& s );
    explicit RenderComponent(std::vector<Sprite> const& ss );
    ~RenderComponent() override = default;

    std::vector<Sprite> sprites;
    int current;
};
