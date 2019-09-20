#pragma once

#include <resource/Sprite.h>
#include <utils/Containers.h>
#include <actions/Action.h>


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

// Do I want to be visible?
struct Render : public Component<Render>
{
    explicit Render( Sprite const& s );
    explicit Render( std::vector<Sprite> const& ss );
    ~Render() override = default;

    std::vector<Sprite> sprites;
    int current;
};

// Do I have a localised position? E.g. I'm not a trigger or a static effect
struct TilePosition : public Component<TilePosition>
{
    explicit TilePosition( Vector2i const& p );
    ~TilePosition() override = default;

    Vector2i position;
};

// Can I block the movement of light or other objects?
struct Collider : public Component<Collider>
{
    explicit Collider() = default;
    ~Collider() override = default;
};

// TBD
struct FixedState : public Component<FixedState>
{
    explicit FixedState( std::vector<std::string> const& states );
    ~FixedState() override = default;

    std::vector<std::string> states;
    int current;
};

// Can I contain items?
struct Container : public Component<Container>
{
    explicit Container( ) = default;
    ~Container() override = default;
};

// Do I have a description?
struct Description : public Component<Description>
{
    explicit Description(std::string val);
    explicit Description(std::vector<std::string> const& vals);
    ~Description() override = default;

    int current;
    std::vector<std::string> descriptions;
};


struct Actor : public Component<Actor>
{
    Actor();
    ~Actor() override = default;

    int initiative;
    int currentEnergy;

    std::unique_ptr<Action> nextAction;
};

}
