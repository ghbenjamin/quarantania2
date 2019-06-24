#include <game/LevelGrid.h>
#include <utils/Assert.h>
#include <algorithm>

LevelGrid::LevelGrid(ImmutableLevelData const &imd)
: m_bounds(imd.levelSize)
{
    m_fixed.reserve( imd.baseTilemap.size() );

    for ( auto t : imd.baseTilemap )
    {
        switch (t)
        {
            case LD::BaseTileType::Entrance:
            case LD::BaseTileType::Wall:
                m_fixed.push_back( Rules::Passibility::Impassable );
                break;

            case LD::BaseTileType::Exit:
            case LD::BaseTileType::Floor:
            case LD::BaseTileType::Junction:
                m_fixed.push_back( Rules::Passibility::Passable );
                break;
        }
    }

    Assert( m_fixed.size() == imd.baseTilemap.size() );
}

void LevelGrid::setPassibility(Rules::Passibility pb, GridRegion const &region, EntityRef ref)
{
    auto pair = std::make_pair(pb, ref);

    for ( auto const& t: region )
    {
        m_dynamic.emplace( t, pair );
    }
}

void LevelGrid::freePassibility(Rules::Passibility pb, GridRegion const &region, EntityRef ref)
{
    auto pair = std::make_pair(pb, ref);

    for ( auto const& t: region )
    {
        auto range_it = m_dynamic.equal_range(t);
        for ( auto it = range_it.first; it != range_it.second; it++ )
        {
            if ( it->second == pair )
            {
                m_dynamic.erase(it);
                break;
            }
        }
    }
}

Rules::Passibility LevelGrid::passibilityAt(Vector2i tile) const
{
    size_t idx = tile.x() + m_bounds.x() * tile.y();

    Rules::Passibility base = m_fixed.at(idx);
    Rules::Passibility dyn = Rules::Passibility::Passable;

    auto range_it = m_dynamic.equal_range( tile );
    for ( auto it = range_it.first; it != range_it.second; it++ )
    {
        if ( static_cast<int>(it->second.first) > static_cast<int>(dyn) )
        {
            dyn = it->second.first;
        }
    }

    return std::max(base, dyn);
}