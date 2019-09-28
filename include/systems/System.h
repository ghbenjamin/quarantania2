#pragma once

#include <memory>

class RenderInterface;
class Level;

class System
{
public:
    explicit System(Level* parent);
    virtual ~System() = default;

    // Actions to be performed every frame
    virtual void update(uint32_t ticks, RenderInterface &rInter);

protected:
    Level* m_level;

};

using SystemPtr = std::unique_ptr<System>;


