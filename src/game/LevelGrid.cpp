#include <game/LevelGrid.h>

LevelGrid::LevelGrid(ImmutableLevelData const &imd)
: m_bounds(imd.levelSize)
{
    m_fixed.reserve( imd.baseTilemap.size() );

    for ( int i = 0;  i < imd.baseTilemap.size(); i++ )
    {
        switch (imd.baseTilemap[i])
        {
            case LD::BaseTileType::Entrance:
            case LD::BaseTileType::Wall:
                m_fixed[i] = Rules::Passibility::Impassable;
                break;

            case LD::BaseTileType::Exit:
            case LD::BaseTileType::Floor:
            case LD::BaseTileType::Junction:
                m_fixed[i] = Rules::Passibility::Passable;
                break;
        }
    }
}

void LevelGrid::setPassibility(Rules::Passibility pb, GridRegion const &region, EntityRef ref)
{

}

void LevelGrid::freePassibility(Rules::Passibility pb, GridRegion const &region, EntityRef ref)
{

}

Rules::Passibility LevelGrid::passibilityAt(Vector2i tile) const
{

}
