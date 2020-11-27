#include <game/Grid.h>
#include <utils/Logging.h>
#include <utils/Assert.h>

Grid::Grid(Vector2i bounds)
: m_bounds(bounds), m_passGrid(m_bounds), m_visGrid(m_bounds), m_lightGrid(m_bounds)
{
}

GridFeature<Passibility, EntityRef> &Grid::pass()
{
    return m_passGrid;
}

GridFeature<Visibility, EntityRef> &Grid::fov()
{
    return m_visGrid;
}

GridFeature<LightLevel, EntityRef> &Grid::light()
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
    return 
		pos.x() >= 0 && 
		pos.y() >= 0 && 
		pos.x() < m_bounds.x() && 
		pos.y() < m_bounds.y();
}

void Grid::calculateFOV(std::vector<FOVObserver> const& sources)
{
    int tileCount = m_bounds.area();

    for ( int i = 0;  i < tileCount; i++ )
    {
        // Reset all the visible tiles to explored tiles
        if (m_visGrid.valueAt(i) != Visibility::Hidden)
        {
            m_visGrid.setFixed(i, Visibility::Explored);
        }
    }

    for ( auto const& source : sources )
    {
        for ( int i = 0; i < 8; i++ )
        {
            FOVWorker(source.source, source.sightLength, 1, 1.0f, 0.0f, &MatrixTransform::squareTransforms[i] );
        }

        m_visGrid.setFixed(source.source, Visibility::Visible);
    }
}

void Grid::FOVWorker(Vector2i source, int maxLength, int row,
                     float start_slope, float end_slope, Matrix2i const* transform)
{
    // Adapted from example at http://www.roguebasin.com/

    if (start_slope < end_slope)
    {
        return;
    }

    float next_start_slope = start_slope;
    for (int i = row; i <= maxLength; i++)
    {
        bool blocked = false;
        for (int dx = -i, dy = -i; dx <= 0; dx++)
        {
            float l_slope = ((float)dx - 0.5f) / ((float)dy + 0.5f);
            float r_slope = ((float)dx + 0.5f) / ((float)dy - 0.5f);
            if (start_slope < r_slope)
            {
                continue;
            }
            else if (end_slope > l_slope)
            {
                break;
            }

            Vector2 tr = transform->transform({dx, dy});

            if ((tr.x() < 0 && std::abs(tr.x()) > source.x())
            || (tr.y() < 0 && std::abs(tr.y()) > source.y()))
            {
                continue;
            }

            Vector2 curr = source + tr;

            if (curr.x() >= m_bounds.x() || curr.y() >= m_bounds.y())
            {
                continue;
            }

            int radius2 = maxLength * maxLength;
            if ( (dx * dx + dy * dy) < radius2 )
            {
                m_visGrid.setFixed(curr, Visibility::Visible);
            }

            if (blocked)
            {
                if ( m_passGrid.valueAt(curr) == Passibility::Impassable )
                {
                    next_start_slope = r_slope;
                    continue;
                }
                else
                {
                    blocked = false;
                    start_slope = next_start_slope;
                }
            }
            else if ( m_passGrid.valueAt(curr) == Passibility::Impassable )
            {
                blocked = true;
                next_start_slope = r_slope;
                FOVWorker(source, maxLength, i + 1, start_slope, l_slope, transform);
            }
        }

        if (blocked)
        {
            break;
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

void Grid::exploreAllTiles()
{
    int tcount = m_bounds.x() * m_bounds.y();

    for ( int i = 0;  i < tcount; i++ )
    {
        m_visGrid.setFixed(i, Visibility::Explored);
    }
}

PathMap const& Grid::availablePathsFromTile(Vector2i source, int maxDistance)
{
    PathMap pm;
    std::vector<Vector2i> queue;

    queue.push_back(source);



    return std::move(pm);
}

