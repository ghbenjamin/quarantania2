#pragma once

#include <game/Component.h>
#include <resource/Sprite.h>

namespace Components
{
struct Render : public Component<Render>
{
    explicit Render( Sprite const& s );
    explicit Render( std::vector<Sprite> const& ss );
    ~Render() override = default;

    std::vector<Sprite> sprites;
    int current;
};
}
