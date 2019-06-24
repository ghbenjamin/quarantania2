#pragma once

#include <memory>

#include <game/GSubber.h>

class RenderInterface;
class Level;
class GEventHub;

class System : public GSubber
{
public:
    explicit System(Level* parent);
    ~System() override = default;

    void acceptGEvent(GEvent &event) override;
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

        void acceptGEvent(GEvent &event) override;
        void update(uint32_t ticks, RenderInterface &rInter) override;
    };

    class Collision : public System
    {
    public:
        explicit Collision(Level *parent);
        ~Collision() override = default;

        void acceptGEvent(GEvent &event) override;
    };

    class Position : public System
    {
    public:
        explicit Position(Level *parent);
        ~Position() override = default;

        void acceptGEvent(GEvent &event) override;
    };
}