#include <game/Grid.h>
#include <utils/Logging.h>
#include <utils/Assert.h>

Grid::Grid(Vector2i bounds)
: m_bounds(bounds), m_passGrid(m_bounds), m_visGrid(m_bounds), m_lightGrid(m_bounds)
{
}

GridFeature<Rules::Passibility, EntityRef> &Grid::pass()
{
    return m_passGrid;
}

GridFeature<Rules::Visibility, EntityRef> &Grid::fov()
{
    return m_visGrid;
}

GridFeature<Rules::LightLevel, EntityRef> &Grid::light()
{
    return m_lightGrid;
}

void Grid::toBounds(GridRegion *region)
{
    region->erase( std::remove_if( region->begin(), region->end(), [this]( auto const& item ){
        return !inBounds(item);
    }), region->end() );
}

bool Grid::inBounds(Vector2i pos)
{
    return pos.x() >= 0 && pos.y() >= 0 && pos.x() < m_bounds.x() && pos.y() < m_bounds.y();
}

void Grid::calculateFOV(Vector2i source, int maxLength)
{
    for ( int i = 0; i < 8; i++ )
    {
        calculateFOVOctant( source, maxLength, 1, 1.0f, 0.0f, i );
    }
}

void
Grid::calculateFOVOctant(Vector2i source, int maxLength, int startColumn, float leftViewSlope, float rightViewSlope,
                         int octant)
{
    AssertNotImplemented();
}

std::vector<EntityRef> Grid::entitiesAtTile(Vector2i pos)
{
    std::vector<EntityRef> out;

    auto range = m_entitiesAtTiles.equal_range( pos );

    for ( auto it = range.first; it != range.second; it++ )
    {
        out.push_back(it->second);
    }

    return out;
}

void Grid::addEntToTile(Vector2i pos, EntityRef ent)
{
    m_entitiesAtTiles.emplace(pos, ent);

    Logging::log( m_entitiesAtTiles.size() );
}

void Grid::removeEntFromTile(Vector2i pos, EntityRef ent)
{
    auto range = m_entitiesAtTiles.equal_range( pos );

    for ( auto it = range.first; it != range.second; )
    {
        if ( it->second == ent )
        {
            it = m_entitiesAtTiles.erase(it);
        }
        else
        {
            it++;
        }
    }
}
