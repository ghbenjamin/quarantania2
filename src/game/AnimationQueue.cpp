#include <game/AnimationQueue.h>
#include <game/Animation.h>

#include <engine/InputInterface.h>
#include <graphics/RenderInterface.h>

AnimationQueue::AnimationQueue()
    : m_isBlocking(false) {}

void AnimationQueue::update( uint32_t ticks, InputInterface &iinter, RenderInterface &rInter )
{
    // Advance our animation state if necessary
    if ( !m_blockingQueue.empty() )
    {
        auto& front = m_blockingQueue.front();
        front->advance(ticks);
        
        if (front->isComplete())
        {
            m_blockingQueue.pop();
            if ( m_blockingQueue.empty() )
            {
                setIsBlocking(false);
            }
        }
    }
}

void AnimationQueue::pushAnimation(std::unique_ptr<Animation> && animation)
{
    setIsBlocking(true);
    m_blockingQueue.push(std::move(animation) );
}

void AnimationQueue::setIsBlocking( bool value)
{
    m_isBlocking = value;
}

bool AnimationQueue::isBlocking() const
{
    return m_isBlocking;
}



