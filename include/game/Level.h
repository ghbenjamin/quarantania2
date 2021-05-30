#pragma once

#include <memory>
#include <set>
#include <vector>

#include <engine/Tiles.h>
#include <engine/GridFeature.h>
#include <engine/Camera.h>
#include <game/GameEvent.h>
#include <game/Player.h>
#include <game/LevelData.h>
#include <game/LevelContext.h>
#include <game/ECS.h>
#include <game/Defines.h>
#include <game/Grid.h>
#include <ui/lib/Manager.h>
#include <game/LevelController.h>
#include <game/GameEventHub.h>

// Forward definitions
class RenderInterface;
class InputInterface;
struct IEvent;


enum class LevelExitStatus
{
    None,
    Completed,
    MainMenu,
    Desktop
};


class Level
{
public:
    explicit Level(Vector2i size, LevelContextPtr ctx, RandomGenerator const& rg);
    virtual ~Level() = default;

    // Indicate that the level is constructed and ready to begin
    void setLayout(LevelLayout const& llayout);
    void setReady();

    // Core
    bool input(IEvent &evt);
    void update(uint32_t ticks, InputInterface& iinter, RenderInterface &rInter);


    // API
    RandomInterface& random();
    Grid& grid();
    GameEventHub& events();
    UI::Manager& ui();
    Camera& camera();
    ECS& ecs();
    LevelController* controller();

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

    // Actions
    std::vector<GameAction> actionsForActor(EntityRef actor);

    // State
    bool isPlayerTurn() const;          // Is the player or the computer taking its turn?
    int getCurrentRound() const;        // The current turn count.
    void switchTurn();                  // Toggle between the player turn and the AI turn
    void advanceRound();                // Move to the next round. Called automatically once the turn toggles twice
    bool isInteractable() const;        // Can the player currently interact with the level?
    void setInteractible(bool value);   // Set whether or not the player is allowed to interact with the level

    // Communication
    LevelExitStatus getLevelExitStatus() const;
    void setLevelExitStatus( LevelExitStatus status );

    std::string getDescriptionForEnt( EntityRef ent );
    std::string getDescriptionForItem( ItemPtr item );


private:

    // UI Methods
    void setupUI();                         // Create our permenant UI elements
    void layoutWindows();                   // Re-layout our dependent UI windows if one of them moves
    RenderObject generateTileRenderData();  // Construct the render object for our tile data
    void centerCameraOnParty();             // Centre the camera on the centroid of the positions of all party members


    // Misc
    RandomInterface m_random;
    LevelContextPtr m_ctx;
    std::vector<std::shared_ptr<LevelController>> m_controllers;
    LevelExitStatus m_exitStatus;

    // Map
    Tileset m_renderTileMap;
    bool m_tileRenderDirtyBit;
    RenderObject m_tileRenderObj;
    std::vector<TileRef> m_mapRendering;
    std::vector<BaseTileType> m_baseTilemap;
    Grid m_grid;
    Camera m_camera;

    // Core
    UI::Manager m_uiManager;
    GameEventHub m_gevents;
    ECS m_ecs;

    // State
    int m_currentRound;  // The turn counter, where one turn is
    bool m_isPlayerTurn; // Is it currently the player's turn?
    bool m_canInteract; // Can the player currently interact with the level?
};

using LevelPtr = std::unique_ptr<Level>;
