#pragma once

#include <memory>

#include <game/GEvent.h>
#include <game/GEventDefs.h>

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
        public GEventSub<GEvents::EntityMove>
    {
    public:
        explicit Collision(Level *parent);
        ~Collision() override = default;

        void accept(GEvents::EntityMove *evt) override;
    };

    class Position : public System,
        public GEventSub<GEvents::EntityMove>
    {
    public:
        explicit Position(Level *parent);
        ~Position() override = default;

        void accept(GEvents::EntityMove *evt) override;
    };
}