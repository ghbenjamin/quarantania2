#pragma once

#include <vector>
#include <unordered_map>

#include <game/LevelData.h>
#include <game/Rules.h>
#include <game/Entity.h>
#include <utils/Containers.h>

using GridRegion = std::vector<Vector2i>;


class LevelGrid
{
public:

    explicit LevelGrid( ImmutableLevelData const& imd );
    ~LevelGrid() = default;

    void setPassibility ( Rules::Passibility pb, GridRegion const& region, EntityRef ref );
    void freePassibility( Rules::Passibility pb, GridRegion const& region, EntityRef ref );

    Rules::Passibility passibilityAt( Vector2i tile ) const;

private:

    Vector2i m_bounds;

    std::vector<Rules::Passibility> m_fixed;
    std::unordered_multimap<Vector2i, std::pair<Rules::Passibility, EntityRef>, Vector2Hash<int>> m_dynamic;


};