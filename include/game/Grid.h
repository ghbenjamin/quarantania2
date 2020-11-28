#pragma once

#include <utils/Containers.h>
#include <engine/Entity.h>
#include <engine/GridFeature.h>
#include <game/Defines.h>

// Something which contributes to the shared field of view of the team, normally a player charatcter.
struct FOVObserver
{
    Vector2i source;
    int sightLength = 0;
};


using PathMap = std::unordered_map<Vector2i, std::pair<Vector2i, float>, Vector2iHash>;

class Grid
{
public:
    explicit Grid( Vector2i bounds );
    ~Grid() = default;

    Grid( const Grid& ) = delete;
    Grid& operator=( const Grid& ) = delete;

    // Data for passibility - how difficult is it to walk over/through each tile?
    GridFeature<Passibility, EntityRef>& pass();

    // Data for field-of-view: how much of each tile can be seen?
    GridFeature<Visibility, EntityRef>& fov();

    // Data for lighting - how light is each tile?
    GridFeature<LightLevel, EntityRef>& light();

    // Cache of which entities are currently in which tile
    std::vector<EntityRef> entitiesAtTile( Vector2i pos ) const;
    void addEntToTile( Vector2i pos, EntityRef ent );
    void removeEntFromTile( Vector2i pos, EntityRef ent );

    // The height and width of our grid (in tiles)
    Vector2i const& bounds() const;

    // Is the specified tile position in bounds?
    bool inBounds( Vector2i pos );

    // Convert to and from a 2d vector position and a 1d array position
    int posToIdx( Vector2i pos );
    Vector2i idxToPos( int idx );

    // Update the FOV of the level with the given observers
    void calculateFOV(std::vector<FOVObserver> const& sources);

    // Mark all tiles as 'explored' (all tiles will be visible but in FOW)
    void exploreAllTiles();


    PathMap allPathsFromTile(Vector2i source, int maxDistance);


private:
    // Adjust the specified grid region so that it is entirely within bounds
    void toBounds( GridRegion* region );

    // Calculate the FOV for a single octant from a single source
    void FOVWorker(Vector2i source, int maxLength, int row, float start_slope,
                   float end_slope, Matrix2i const* transform);



    Vector2i m_bounds;

    GridFeature<Passibility, EntityRef> m_passGrid;
    GridFeature<LightLevel, EntityRef> m_lightGrid;
    GridFeature<Visibility, EntityRef> m_visGrid;

    std::unordered_multimap<Vector2i, EntityRef, Vector2iHash> m_entitiesAtTiles;

};