#pragma once

#include <memory>

class RenderInterface;
class ECS;

class System
{
public:
    explicit System(ECS* parent);
    virtual ~System() = default;

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

        void update(uint32_t ticks, RenderInterface &rInter) override;

    private:
    };
}