#pragma once

#include <memory>

#include <game/GSubber.h>

class RenderInterface;
class ECS;
class GEventHub;

class System : public GSubber
{
public:
    explicit System(ECS* parent);
    ~System() override = default;

    void acceptGEvent(GEvent &event) override;
    virtual void update(uint32_t ticks, RenderInterface &rInter);

protected:
    ECS* m_ecs;

};

using SystemPtr = std::unique_ptr<System>;



namespace Systems
{
    class Render : public System
    {
    public:
        explicit Render(ECS *parent);
        ~Render() override = default;

        void acceptGEvent(GEvent &event) override;
        void update(uint32_t ticks, RenderInterface &rInter) override;
    };

    class Collision : public System
    {
    public:
        explicit Collision(ECS *parent);
        ~Collision() override = default;

        void acceptGEvent(GEvent &event) override;
    };

    class Position : public System
    {
    public:
        explicit Position(ECS *parent);
        ~Position() override = default;

        void acceptGEvent(GEvent &event) override;
    };
}