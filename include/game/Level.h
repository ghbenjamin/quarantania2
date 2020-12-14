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
#include <controllers/LevelController.h>
#include <ui/Manager.h>


// Forward definitions
class RenderInterface;
class InputInterface;
struct IEvent;
struct IEventKeyPress;
struct IEventMouseMove;
struct IEventMouseDown;
struct IEventWindowResize;
class Action;
class Action;

namespace UI
{
    class TextLog;
}

// Typedefs 
using ActionPtr = std::shared_ptr<Action>;

class Level
{
public:
    explicit Level(Vector2i size, LevelContextPtr ctx, RandomGenerator const& rg);
    virtual ~Level() = default;

    // Indicate that the level is constructed and ready to begin
    void setLayout(LD::LevelLayout const& llayout);
    void setReady();

    // Core
    bool input(IEvent &evt);
    void update(uint32_t ticks, InputInterface& iinter, RenderInterface &rInter);

    RandomInterface& random();
    Grid& grid();
    GEventHub& events();
    UI::Manager& ui();
    Camera& camera();
    ECS& ecs();

    // Coordinates
    Vector2i worldCoordsToScreen( Vector2i const& world );
    Vector2i screenCoordsToWorld( Vector2i const& screen );
    Vector2i worldCoordsToTile( Vector2i const& world);
    Vector2i screenCoordsToTile( Vector2i const& screen);
    Vector2i tileCoordsToScreen( Vector2i const& tile );

    int squaredEntityDistance(EntityRef a, EntityRef b);

    // Actions
//    std::vector<ActionPtr> actionsForTile(EntityRef actor, Vector2i tile);
//    std::vector<ActionPtr> actionsForEntity(EntityRef actor, EntityRef subject);
//    std::vector<ActionPtr> actionsForPosition(EntityRef actor, Vector2i position);

    std::vector<std::shared_ptr<Action>> actionsForActor(EntityRef actor );


    // Communication
    bool isComplete() const;
    void setComplete();

    void addTextLogMessage( std::string_view sv, Colour const& colour );
    void addTextLogMessage( std::string_view sv );

    std::string_view getDescriptionForEnt( EntityRef ent );
    std::string_view getDescriptionForItem( ItemPtr item );

    // Game Logic

    // The list of entities in the level, in turn order
    std::vector<EntityRef> const& turnOrder() const;

    // The entity whose turn it is
    EntityRef getActiveEntity() const;

    // Mark that the current entity has finished its turn, and that the turn should be passed to the next
    // entity in the queue.
    void nextTurn();

private:

    // UI Methods
    void setupUI();
    void render(uint32_t ticks, InputInterface& iinter, RenderInterface &rInter);
    void renderTiles(uint32_t ticks, RenderInterface &rInter);
    void layoutWindows();

    // Game logic methods
    void generateTurnOrder();


    // Misc
    RandomInterface m_random;
    LevelContextPtr m_ctx;
    std::vector<std::shared_ptr<Controller>> m_controllers;
    bool m_isComplete;

    // Map
    Tileset m_renderTileMap;
    std::vector<TileRef> m_mapRendering;
    std::vector<LD::BaseTileType> m_baseTilemap;
    Grid m_grid;
    Camera m_camera;

    // Game logic
    std::vector<EntityRef> m_turnOrder;

    // Core
    UI::Manager m_uiManager;
    GEventHub m_gevents;
    ECS m_ecs;


    std::shared_ptr<UI::TextLog> m_textLog;

    // Tuns
    EntityRef m_currentTurnEntity;
};

using LevelPtr = std::unique_ptr<Level>;
