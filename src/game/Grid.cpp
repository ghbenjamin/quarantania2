#include <game/Grid.h>

#include <algorithm>

#include <utils/Logging.h>
#include <utils/Assert.h>
#include <queue>

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

GridFeature<float, EntityRef, float> &Grid::light()
{
    return m_lightGrid;
}

void Grid::toBounds(GridRegion *region)
{
    region->erase( std::remove_if( region->begin(), region->end(), [this]( auto const& item ){
        return !inBounds(item);
    }), region->end() );
}

bool Grid::inBounds(Vector2i pos) const
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
        // For each source, sweep 8 right-triangular octants of the level starting from the source tile, working out
        // which tiles are visible and which are not.
        for ( int i = 0; i < 8; i++ )
        {
            FOVWorker(source.source, source.sightLength, 1, 1.0f, 0.0f, &MatrixTransform::squareTransforms[i] );
        }

        // Make sure to include the origin tile itself in the visible tiles
        m_visGrid.setFixed(source.source, Visibility::Visible);
    }
}

void Grid::calculateLightLevel( std::vector<LightSource> const &sources )
{
    for ( int i = 0;  i < m_bounds.area(); i++ )
    {
        m_lightGrid.setFixed(i, 0.0f);
    }
    
    for ( auto const& source : sources )
    {
        for ( int i = 0; i < 8; i++ )
        {
            static const float LightCalcLength = 5.0f;
            LightMapWorker(source.source, source.intensity, LightCalcLength,
                1, 1.0f, 0.0f, &MatrixTransform::squareTransforms[i] );
        }
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

            if ((tr.x() < 0 && std::abs(tr.x()) > source.x()) || (tr.y() < 0 && std::abs(tr.y()) > source.y()))
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


void Grid::LightMapWorker( Vector2i source, float intensity, float maxLength, int row, float start_slope, float end_slope,
                           Matrix2i const *transform )
{
    float maxRadius2 = maxLength * maxLength;

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
            
            if ((tr.x() < 0 && std::abs(tr.x()) > source.x()) || (tr.y() < 0 && std::abs(tr.y()) > source.y()))
            {
                continue;
            }
            
            Vector2 curr = source + tr;
            
            if (curr.x() >= m_bounds.x() || curr.y() >= m_bounds.y())
            {
                continue;
            }
            
            float radius2 = (float)dx * dx + (float)dy * dy;
            if (radius2 < maxRadius2 )
            {
                // Don't light up regions of the map that we can't see
                if (m_visGrid.valueAt(curr) != Visibility::Visible)
                {
                    return;
                }
    
                // Set the current light level as a function of how far away we are from the light we're considering
                // Linearly combine contributions from different lights
                float currLightVal = intensity * 0.4f / std::sqrt(radius2);
                float oldLightVal = m_lightGrid.valueAt(curr);
                m_lightGrid.setFixed( curr, currLightVal + oldLightVal );
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
                LightMapWorker(source, intensity, maxLength, i + 1, start_slope, l_slope, transform);
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


EntityRef Grid::firstEntityAtTile( Vector2i pos ) const
{
    auto range = m_entitiesAtTiles.equal_range( pos );
    
    for ( auto it = range.first; it != range.second; it++ )
    {
        return it->second;
    }
    
    return EntityNull;
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

Vector2i Grid::idxToPos(int idx) const
{
    return { idx % m_bounds.x(), idx / m_bounds.x() };
}

int Grid::posToIdx(Vector2i pos) const
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

PathMap Grid::allPathsFromTile(Vector2i source, int maxDistance)
{
    std::list<Vector2i> queue;
    std::unordered_map<Vector2i, float, Vector2iHash> weights;
    std::unordered_map<Vector2i, Vector2i, Vector2iHash> parents;

    queue.push_back(source);
    weights[source] = 0;

    while ( !queue.empty() )
    {
        auto curr = queue.back();
        queue.pop_back();
        auto curr_weight = weights[curr];

        for (auto const&[dir, nn] : GridUtils::AllNeighbours)
        {
            auto next = nn + curr;
            float dw = GridUtils::isAdjacentCardinal(curr, next) ? 1.0f : 1.41f;
            auto next_weight = curr_weight + dw;
    
            if (!inBounds(next))
            {
                // Don't walk off of the map
                continue;
            }

            if (m_passGrid.valueAt(next) != Passibility::Passable)
            {
                // Don't walk through walls
                continue;
            }

            if (next_weight > maxDistance)
            {
                // Don't walk too far
                continue;
            }

            auto existing_weight = weights.find(next);
            if (existing_weight == weights.end())
            {
                // New node!
                parents[next] = curr;
                weights[next] = next_weight;
                queue.push_back(next);
            }
            else
            {
                // Old node, but faster path
                if (next_weight < existing_weight->second)
                {
                    parents[next] = curr;
                    weights[next] = next_weight;
                    queue.push_back(next);
                }
            }
        }
    }

    PathMap pm;

    for ( auto const& [k, v] : parents )
    {
        pm[k] = { v, weights[k] };
    }

    return std::move(pm);
}

std::vector<Vector2i> Grid::pathFromPathMap(const PathMap &map, Vector2i tile)
{
    std::vector<Vector2i> path;
    Vector2i curr = tile;
    auto it = map.find(curr);

    for (; it != map.end(); it = map.find(curr))
    {
        path.push_back(curr);
        curr = it->second.first;
    }

    std::reverse( path.begin(), path.end() );

    return std::move(path);
}

std::vector<std::pair<Vector2i, float>> Grid::pathBetweenPoints( Vector2i source, Vector2i dest )
{
    using VecPair = std::pair<Vector2i, float>;
    auto VecPairCmp = []( VecPair const& lhs, VecPair const& rhs ){ return lhs.second > rhs.second; };
    auto heuristic = []( Vector2i const& lhs, Vector2i const& rhs ){ return std::abs(rhs.x() - lhs.x()) + std::abs(rhs.y() - lhs.y()); };
    
    std::vector<std::pair<Vector2i, float>> out;
    std::priority_queue<VecPair, std::vector<VecPair>, decltype(VecPairCmp)> openSet( VecPairCmp );
    std::unordered_map<Vector2i, float, Vector2iHash> costs;
    std::unordered_map<Vector2i, Vector2i, Vector2iHash> cameFrom;
    
    
    openSet.push( std::make_pair(source, 0.f) );
    costs[source] = 0.f;

    while (!openSet.empty())
    {
        auto current = openSet.top();
        openSet.pop();
        
        if (current.first == dest)
        {
            break;
        }
        
        for (auto const&[dir, nn] : GridUtils::AllNeighbours)
        {
            auto next = current.first + nn;
            float deltaCost = GridUtils::isAdjacentCardinal(current.first, next) ? 1.0f : 1.41f;
            auto newCost = costs[current.first] + deltaCost;
    
            // Don't walk off of the map
            if (!inBounds(next))
            {
                continue;
            }
    
            // Don't walk through walls
            if (m_passGrid.valueAt(next) != Passibility::Passable && next != dest)
            {
                continue;
            }
            
            auto nextInCosts = costs.find(next);
            if ( nextInCosts == costs.end() || newCost < costs[next] )
            {
                costs[next] = newCost;
                float priority = newCost + heuristic( next, dest );
                
                openSet.push( std::make_pair(next, priority) );
                cameFrom[next] = current.first;
            }
        }
    }
    
    // Convert our map of to -> from nodes to a linear path, making sure both the start and
    // end nodes are on the path
    
    Vector2i curr = dest;
    auto it = cameFrom.find(curr);

    // Endpoint not in the map => there's no path between the specified points
    if (it == cameFrom.end())
    {
        return {};
    }
    
    for (; it != cameFrom.end(); it = cameFrom.find(curr))
    {
        out.push_back(std::make_pair(curr, costs[curr]));
        curr = it->second;
    }
    
    out.push_back( std::make_pair(source, 0.0f) );
    std::reverse( out.begin(), out.end() );

    return out;
}



