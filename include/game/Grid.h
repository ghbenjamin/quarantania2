#pragma once

#include <utils/Containers.h>
#include <game/Entity.h>
#include <game/GridFeature.h>
#include <game/Rules.h>

class Grid
{
public:
    explicit Grid( Vector2i bounds );
    ~Grid() = default;

    Grid( const Grid& ) = delete;
    Grid& operator=( const Grid& ) = delete;

    GridFeature<Rules::Passibility, EntityRef>& pass();
    GridFeature<Rules::Visibility, EntityRef>& fov();
    GridFeature<Rules::LightLevel, EntityRef>& light();

    std::vector<EntityRef> entitiesAtTile( Vector2i pos ) const;
    void addEntToTile( Vector2i pos, EntityRef ent );
    void removeEntFromTile( Vector2i pos, EntityRef ent );

    Vector2i const& bounds() const;
    bool inBounds( Vector2i pos );
    void recalculateFOV( Vector2i source, int maxLength );

    inline int posToIdx( Vector2i pos );
    inline Vector2i idxToPos( int idx );

private:
    void toBounds( GridRegion* region );



private:
    Vector2i m_bounds;

    GridFeature<Rules::Passibility, EntityRef> m_passGrid;
    GridFeature<Rules::LightLevel, EntityRef> m_lightGrid;
    GridFeature<Rules::Visibility, EntityRef> m_visGrid;

    std::unordered_multimap<Vector2i, EntityRef, Vector2iHash> m_entitiesAtTiles;

};