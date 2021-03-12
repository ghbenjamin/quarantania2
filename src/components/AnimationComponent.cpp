#include <components/AnimationComponent.h>
#include <utils/Assert.h>
#include <utils/Logging.h>

TileAnimationPath::TileAnimationPath( std::vector<Vector2f> const &path, float timePerStep )
    : m_path(path), m_timePerStep(timePerStep), m_isComplete(false),
      m_currIdx(1), m_interpolator(path[0], path[1], timePerStep)
{
    AssertMsg( path.size() >= 2, "Animation path requires at least two points" );
}

Vector2f TileAnimationPath::currentPosition() const
{
    return m_interpolator.current();
}

bool TileAnimationPath::isComplete() const
{
    return m_isComplete;
}

void TileAnimationPath::advance( std::uint32_t ticks )
{
    m_interpolator.advance( ticks );
    
    if ( m_interpolator.isFinished() )
    {
        if ( m_currIdx + 1 == m_path.size() )
        {
            m_isComplete = true;
            return;
        }
        else
        {
            m_currIdx++;
            m_interpolator = { m_path[m_currIdx-1], m_path[m_currIdx], m_timePerStep };
        }
    }
}

Vector2f TileAnimationPath::finalPosition() const
{
    return m_path.back();
}

ColourModification::ColourModification( Colour colour, float seconds )
 : m_maxSeconds(seconds), m_colour(colour), m_currSeconds(0.0f), m_isComplete(false) {}

void ColourModification::advance( std::uint32_t ticks )
{
    m_currSeconds += ((float)ticks / 1000.0f);
    
    if (m_currSeconds > m_maxSeconds)
    {
        m_isComplete = true;
    }
}

Colour const &ColourModification::colour()
{
    return m_colour;
}

bool ColourModification::isComplete() const
{
    return m_isComplete;
}
