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
    
    if ( m_currentPxPos.x() > m_nextTile.x() )
    {
        advanceTile();
    }
}

void TileAnimationPath::advanceTile()
{
    if ( m_currIdx + 1 == m_path.size() )
    {
        Logging::log ( "Done!\n" );
        m_isComplete = true;
        return;
    }

    m_prevTile = m_path[m_currIdx];
    m_currIdx++;
    m_nextTile = m_path[m_currIdx];
    
    Logging::log ( "Advancing from tile {} to {}\n", m_prevTile.to_string(), m_nextTile.to_string() );
    
    Vector2f tileDelta = (m_nextTile - m_prevTile);
    float ticksPerStep = m_timePerStep * 1000;
    
    m_deltaPerTick = tileDelta / ticksPerStep;
    m_currentPxPos = m_prevTile;
}
