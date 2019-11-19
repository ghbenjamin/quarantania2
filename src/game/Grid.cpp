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

void Grid::recalculateFOV(Vector2i source, int maxLength)
{
    int tcount = m_bounds.x() * m_bounds.y();

    // Debug: simple circle based FOV
    for ( int i = 0;  i < tcount; i++ )
    {
        // Reset all the visible tiles to explored tiles
        if (m_visGrid.valueAt(i) != Rules::Visibility::Hidden)
        {
            m_visGrid.setFixed(i, Rules::Visibility::Explored);
        }
    }

    // Work out which tiles are now visible

    int c2 = maxLength * maxLength;
    for ( int i = 0;  i < tcount; i++ )
    {
        auto dv = source - idxToPos(i);
        if ( dv.x() * dv.x() + dv.y() * dv.y() <= c2 )
        {
            m_visGrid.setFixed(i, Rules::Visibility::Visible);
        }
    }
}


std::vector<EntityRef> Grid::entitiesAtTile(Vector2i pos) const
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

Vector2i Grid::idxToPos(int idx)
{
    return { idx % m_bounds.x(), idx / m_bounds.x() };
}

int Grid::posToIdx(Vector2i pos)
{
    return pos.x() + pos.y() * m_bounds.x();
}

Vector2i const &Grid::bounds() const
{
    return m_bounds;
}
