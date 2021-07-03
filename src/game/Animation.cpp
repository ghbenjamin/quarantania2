#include <game/Animation.h>
#include <game/Level.h>
#include <game/ECS.h>
#include <components/PositionComponent.h>
#include <components/RenderComponent.h>

Animation::Animation( Level* level, float totalTime )
    : m_level(level), m_isComplete(false), m_totalTime(totalTime), m_currentTime(0.0f), m_hasStarted(false)
    {}

bool Animation::isComplete() const
{
    return m_isComplete;
}

void Animation::advance(std::uint32_t ticks)
{
    if (!m_hasStarted)
    {
        m_hasStarted = true;
        onStart();
    }

    m_currentTime += ((float) ticks / 1000.0f);

    if (m_currentTime > m_totalTime)
    {
        m_isComplete = true;
    }

    advanceSelf(ticks);
}


AnimTilePath::AnimTilePath(Level* level, EntityRef entity, const std::vector<Vector2f> &path, float timePerStep)
   : Animation( level, timePerStep * path.size() ),
     m_path(path), m_entity(entity),
     m_currIdx(1), m_interpolator(path[0], path[1], timePerStep),
     m_timePerStep(timePerStep)
{
}

void AnimTilePath::advanceSelf(std::uint32_t ticks)
{
    m_interpolator.advance( ticks );

    if ( m_interpolator.isFinished() )
    {
        if ( m_currIdx + 1 == m_path.size() )
        {
            m_isComplete = true;
        }
        else
        {
            m_currIdx++;
            m_interpolator = { m_path[m_currIdx-1], m_path[m_currIdx], m_timePerStep };
        }
    }

    auto posC = m_level->ecs().getComponents<PositionComponent>(m_entity);

    posC->pixelPosition = m_interpolator.current().convert<int>();

    if ( m_isComplete )
    {
        posC->pixelPosition = m_path.back().convert<int>();
    }
    else
    {
        posC->pixelPosition = m_interpolator.current().convert<int>();
    }
}

AnimColourMod::AnimColourMod(Level* level, EntityRef entity, Colour startColour, Colour endColour, float seconds)
   : Animation(level, seconds), m_entity(entity), m_startColour(startColour), m_endColour(endColour) {}

void AnimColourMod::advanceSelf(std::uint32_t ticks)
{
    if (m_isComplete)
    {
        auto animC = m_level->ecs().getComponents<RenderComponent>(m_entity);
        animC->sprite.resetColourMod();
    }
}

void AnimColourMod::onStart()
{
    auto animC = m_level->ecs().getComponents<RenderComponent>(m_entity);
    animC->sprite.setColourMod(m_startColour);
}


AnimLinearSpline::AnimLinearSpline(Level *level, EntityRef entity, Vector2i target, const Spline &spline, float seconds)
    : Animation(level, seconds), m_entity(entity), m_target(target), m_spline(spline)
{
    auto posC = m_level->ecs().getComponents<PositionComponent>(m_entity);

    auto delta = target - posC->tilePosition;
}

void AnimLinearSpline::advanceSelf(std::uint32_t ticks)
{

}