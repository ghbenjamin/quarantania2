#pragma once

#include <resource/Sprite.h>
#include <utils/Containers.h>

struct Component
{
};

using ComponentPtr = std::shared_ptr<Component>;


namespace Components
{

struct Render : public Component
{
    explicit Render( Sprite const& s ) : sprite(s) {}
    ~Render() = default;

    Sprite sprite;
};

struct TilePosition : public Component
{
    explicit TilePosition( Vector2i const& p ) : position(p) {}
    ~TilePosition() = default;
    Vector2i position;
};

struct Collider : public Component
{
    explicit Collider() {}
    ~Collider() = default;
};



}
