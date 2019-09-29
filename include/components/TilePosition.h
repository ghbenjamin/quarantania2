#pragma once

#include <game/Component.h>
#include <utils/Containers.h>

namespace Components
{

// Do I have a localised position? E.g. I'm not a trigger or a static effect
struct TilePosition : public Component<TilePosition>
{
    explicit TilePosition( Vector2i const& p );
    ~TilePosition() override = default;

    Vector2i position;
};


}
