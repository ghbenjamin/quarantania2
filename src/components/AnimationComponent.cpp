#include <components/AnimationComponent.h>
#include <utils/Assert.h>
#include <utils/Logging.h>

TileAnimationPath::TileAnimationPath( std::vector<Vector2f> const &path, double totalAnimationTime )
    : m_path(path), m_totalAnimationTime(totalAnimationTime), m_isComplete(false), m_currIdx(0)
{
    AssertMsg( path.size() >= 2, "Animation path requires at least two points" );
    
    m_timePerStep =  totalAnimationTime / (path.size() - 1);
    m_currentPxPos = path[0];
    
    m_prevTile = path[1];
    m_nextTile = path[1];
    
    advanceTile();
}

Vector2f TileAnimationPath::currentPosition() const
{
    return m_currentPxPos;
}

bool TileAnimationPath::isComplete() const
{
    return m_isComplete;
}

void TileAnimationPath::advance( std::uint32_t ticks )
{
    if (m_isComplete)
    {
        return;
    }
    
    Vector2f delta = m_deltaPerTick * (float) ticks;
    m_currentPxPos += delta;

    bool shouldAdvance =
            ( m_deltaPerTick.x() > 0 && m_currentPxPos.x() > m_nextTile.x() ) ||
            ( m_deltaPerTick.x() < 0 && m_currentPxPos.x() < m_nextTile.x() ) ||
            ( m_deltaPerTick.y() > 0 && m_currentPxPos.y() > m_nextTile.y() ) ||
            ( m_deltaPerTick.y() < 0 && m_currentPxPos.y() < m_nextTile.y() );

    if ( shouldAdvance )
    {
        advanceTile();
    }
}

void TileAnimationPath::advanceTile()
{
    if ( m_currIdx + 1 == m_path.size() )
    {
        m_isComplete = true;
        return;
    }

    m_prevTile = m_path[m_currIdx];
    m_currIdx++;
    m_nextTile = m_path[m_currIdx];

    Vector2f tileDelta = (m_nextTile - m_prevTile);
    float ticksPerStep = m_timePerStep * 1000;
    
    m_deltaPerTick = tileDelta / ticksPerStep;
    m_currentPxPos = m_prevTile;
}

Vector2f TileAnimationPath::finalPosition() const
{
    return m_path.back();
}
