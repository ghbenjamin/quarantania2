#pragma once

#include <memory>

#include <game/GEvent.h>
#include <game/GEventDefs.h>
#include <resource/Sprite.h>

class RenderInterface;
class Level;

class System
{
public:
    explicit System(Level* parent);
    virtual ~System() = default;

    virtual void update(uint32_t ticks, RenderInterface &rInter);

protected:
    Level* m_level;

};

using SystemPtr = std::unique_ptr<System>;



namespace Systems
{


class Render : public System
{
public:
    explicit Render(Level *parent);
    ~Render() override = default;

    void update(uint32_t ticks, RenderInterface &rInter) override;
};

class Collision : public System,
    public GEventSub<GEvents::EntityMove>,
    public GEventSub<GEvents::EntityReady>
{
public:
    explicit Collision(Level *parent);
    ~Collision() override = default;

    void accept(GEvents::EntityMove *evt) override;
    void accept(GEvents::EntityReady *evt) override;
};

class Position : public System,
    public GEventSub<GEvents::EntityMove>,
    public GEventSub<GEvents::EntityReady>
{
public:
    explicit Position(Level *parent);
    ~Position() override = default;

    void accept(GEvents::EntityMove *evt) override;
    void accept(GEvents::EntityReady *evt) override;
};

class FOV : public System,
                 public GEventSub<GEvents::EntityMove>
{
public:
    explicit FOV(Level *parent);
    ~FOV() override = default;

    void update(uint32_t ticks, RenderInterface &rInter) override;
    void accept(GEvents::EntityMove *evt) override;

private:
    Sprite m_fovHidden;
    Sprite m_fovFog;
};

class FixedState : public System,
                            public GEventSub<GEvents::FixedStateChange>
{
public:
    explicit FixedState(Level *parent);
    ~FixedState() override = default;

    void accept(GEvents::FixedStateChange *evt) override;
};

}