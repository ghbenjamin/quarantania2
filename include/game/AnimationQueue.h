#pragma once

#include <queue>
#include <memory>

#include <game/Animation.h>

// Forward definitions
class RenderInterface;
class InputInterface;

class AnimationQueue
{
public:
    AnimationQueue();
    ~AnimationQueue() = default;
    
    void update(uint32_t ticks, InputInterface& iinter, RenderInterface &rInter);
    void pushAnimation( std::unique_ptr<Animation>&& animation );
    void setIsBlocking( bool value);
    bool isBlocking() const;

private:
    std::queue<std::unique_ptr<Animation>> m_blockingQueue;
    bool m_isBlocking;
};