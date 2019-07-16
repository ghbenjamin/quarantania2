#pragma once

#include <resource/Sprite.h>
#include <utils/Containers.h>


using ComponentId = std::size_t;

struct BaseComponent
{
public:
    BaseComponent() = default;
    virtual ~BaseComponent() = default;

protected:
    static ComponentId m_globalId;
};

template <typename ET>
struct Component : public BaseComponent
{
    Component() = default;
    ~Component() override = default;

    static ComponentId id()
    {
        static ComponentId m_id = m_globalId++;
        return m_id;
    }
};


namespace Components
{

struct Render : public Component<Render>
{
    explicit Render( Sprite const& s ) : sprite(s) {}
    ~Render() override = default;

    Sprite sprite;
};

struct TilePosition : public Component<TilePosition>
{
    explicit TilePosition( Vector2i const& p ) : position(p) {}
    ~TilePosition() override = default;

    Vector2i position;
};

struct Collider : public Component<Collider>
{
    explicit Collider() = default;
    ~Collider() override = default;
};

struct FixedState : public Component<FixedState>
{
    explicit FixedState( int start ) : state(start) {}
    ~FixedState() override = default;

    int state;
};

struct FixedRenderState : public Component<FixedRenderState>
{
    explicit FixedRenderState( ) = default;
    ~FixedRenderState() override = default;

    Sprite states[8];
};


}
