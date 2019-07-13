#include <game/Grid.h>
#include <utils/Logging.h>

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
        return !(item.x() >= 0 && item.y() >= 0 and item.x() < m_bounds.x() && item.y() < m_bounds.y());
    }), region->end() );
}

void Grid::calculateFOV(Vector2i source, int maxLength)
{

}
