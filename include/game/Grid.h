#pragma once

#include <utils/Containers.h>
#include <engine/Entity.h>
#include <engine/GridFeature.h>
#include <game/Defines.h>

class Grid
{
public:
    explicit Grid( Vector2i bounds );
    ~Grid() = default;

    Grid( const Grid& ) = delete;
    Grid& operator=( const Grid& ) = delete;

    GridFeature<Passibility, EntityRef>& pass();
    GridFeature<Visibility, EntityRef>& fov();
    GridFeature<LightLevel, EntityRef>& light();

    std::vector<EntityRef> entitiesAtTile( Vector2i pos ) const;
    void addEntToTile( Vector2i pos, EntityRef ent );
    void removeEntFromTile( Vector2i pos, EntityRef ent );

    Vector2i const& bounds() const;
    bool inBounds( Vector2i pos );

    void calculateFOV(Vector2i source, int maxLength);
    void calculateFOVMulti(std::vector<Vector2i> const& sources, int maxLength);
    void FOVWorker(Vector2i source, int maxLength, int row, float start_slope,
                   float end_slope, Matrix2i const* transform);

    void exploreAllTiles();

    int posToIdx( Vector2i pos );
    Vector2i idxToPos( int idx );

private:
    void toBounds( GridRegion* region );


    Vector2i m_bounds;

    GridFeature<Passibility, EntityRef> m_passGrid;
    GridFeature<LightLevel, EntityRef> m_lightGrid;
    GridFeature<Visibility, EntityRef> m_visGrid;

    std::unordered_multimap<Vector2i, EntityRef, Vector2iHash> m_entitiesAtTiles;

};