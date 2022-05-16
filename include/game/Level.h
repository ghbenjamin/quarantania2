#pragma once

#include <memory>
#include <set>
#include <vector>

#include <engine/Tiles.h>
#include <engine/GridFeature.h>
#include <engine/Camera.h>
#include <game/GameEvent.h>
#include <game/LevelData.h>
#include <game/ECS.h>
#include <game/Defines.h>
#include <game/Grid.h>
#include <game/GameEventHub.h>
#include <game/AnimationQueue.h>

// Forward definitions
class RenderInterface;
class InputInterface;
struct IEvent;
class LuaState;

enum class LevelExitStatus
{
    None,
    Completed,
    MainMenu,
    Desktop
};

struct LevelInitData
{
    Vector2i size;
};

class Level
{
public:
    Level(LevelInitData& data, LuaState& lua, RandomState* randomState);
    virtual ~Level() = default;
    static void setLuaType( LuaState& lua );


    // Indicate that the level is constructed and ready to begin
    void setLayout(LevelLayout const& llayout);
    void setReady();

    // Core
    bool input(IEvent &evt);
    void update(uint32_t ticks, InputInterface& iinter, RenderInterface &rInter);

    // API
    RandomState* random();
    Grid& grid();
    GameEventHub& events();
    Camera& camera();
    ECS& ecs();
    AnimationQueue& animation();

    // Coordinates
    Vector2i worldCoordsToScreen( Vector2i const& world ) const;
    Vector2i screenCoordsToWorld( Vector2i const& screen ) const;
    Vector2i worldCoordsToTile( Vector2i const& world) const;
    Vector2i screenCoordsToTile( Vector2i const& screen) const;
    Vector2i tileCoordsToScreen( Vector2i const& tile ) const;
    Vector2i tileCoordsToWorld( Vector2i const& tile ) const;
    Vector2i tileIdxToWorld( int idx ) const;

    int squaredEntityDistance(EntityRef a, EntityRef b);
    float entityDistance(EntityRef a, EntityRef b);

    // State
    bool isPlayerTurn() const;          // Is the player or the computer taking its turn?
    int getCurrentRound() const;        // The current turn count.
    void switchTurn();                  // Toggle between the player turn and the AI turn
    void advanceRound();                // Move to the next round. Called automatically once the turn toggles twice

    // Communication
    LevelExitStatus getExitStatus() const;
    void setExitStatus( LevelExitStatus status );

    // Helpful functions
    std::string getDescriptionForEnt( EntityRef ent );
    std::string getDescriptionForItem( ItemPtr item );

    int partyRemainingCount();
    int enemiesRemainingCount();


private:

    // UI Methods
    void layoutWindows();                   // Re-layout our dependent UI windows if one of them moves
    RenderObject generateTileRenderData();  // Construct the render object for our tile data
    void centerCameraOnParty();             // Centre the camera on the centroid of the positions of all party members
    void loadScripts();

    // Misc
    RandomState* m_randomState;
    LuaState& m_lua;
    LevelExitStatus m_exitStatus;

    // Map
    Tileset m_renderTileMap;
    bool m_tileRenderDirtyBit;
    RenderObject m_tileRenderObj;
    std::vector<TileRef> m_mapRendering;
    std::vector<BaseTileType> m_baseTilemap;
    Grid m_grid;
    Camera m_camera;
    
    // Animation
    AnimationQueue m_animationQueue;
    
    // Core
    GameEventHub m_gevents;
    ECS m_ecs;

    // State
    int m_currentRound;
    bool m_isPlayerTurn; // Is it currently the player's turn?
};

using LevelPtr = std::unique_ptr<Level>;
