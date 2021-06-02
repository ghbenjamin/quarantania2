#pragma once

#include <utils/Containers.h>
#include <engine/Entity.h>
#include <engine/GridFeature.h>
#include <game/Defines.h>
#include <utils/Colour.h>

// Something which contributes to the shared field of view of the team, normally a player charatcter.
struct FOVObserver
{
    Vector2i source;
    int sightLength = 0;
};

struct LightSource
{
    Vector2i source;
    float intensity;
    Colour colour;
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
    GridFeature<float, EntityRef, float>& light();

    // Cache of which entities are currently in which tile
    std::vector<EntityRef> entitiesAtTile( Vector2i pos ) const;
    void addEntToTile( Vector2i pos, EntityRef ent );
    void removeEntFromTile( Vector2i pos, EntityRef ent );

    // The height and width of our grid (in tiles)
    Vector2i const& bounds() const;

    // Is the specified tile position in bounds?
    bool inBounds( Vector2i pos ) const;

    // Convert to and from a 2d vector position and a 1d array position
    int posToIdx( Vector2i pos ) const;
    Vector2i idxToPos( int idx ) const;

    // Update the FOV of the level with the given observers
    void calculateFOV(std::vector<FOVObserver> const& sources);

    // Update the lighting of the level with the given light sources
    void calculateLightLevel( std::vector<LightSource> const& sources );

    // Mark all tiles as 'explored' (all tiles will be visible but in FOW)
    void exploreAllTiles();
    
    PathMap allPathsFromTile(Vector2i source, int maxDistance);
    std::vector<Vector2i> pathFromPathMap(PathMap const& map, Vector2i tile);

    // Find the fastest path between two points
    std::vector<std::pair<Vector2i, float>> pathBetweenPoints( Vector2i source, Vector2i dest );


private:
    // Adjust the specified grid region so that it is entirely within bounds
    void toBounds( GridRegion* region );

    // Calculate the FOV for a single octant from a single source
    void FOVWorker(Vector2i source, int maxLength, int row, float start_slope,
        float end_slope, Matrix2i const* transform);

    // Calculate the FOV for a single octant from a single source
    void LightMapWorker(Vector2i source, float intensity, float maxLength, int row, float start_slope,
                   float end_slope, Matrix2i const* transform);



    Vector2i m_bounds;

    GridFeature<Passibility, EntityRef> m_passGrid;
    GridFeature<float, EntityRef, float> m_lightGrid;
    GridFeature<Visibility, EntityRef> m_visGrid;

    std::unordered_multimap<Vector2i, EntityRef, Vector2iHash> m_entitiesAtTiles;

};