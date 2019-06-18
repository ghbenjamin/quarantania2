#pragma once

#include <memory>
#include <vector>

#include <SDL2/SDL.h>

#include <game/ECS.h>
#include <game/Tiles.h>
#include <game/Player.h>
#include <game/EntityFactory.h>

class RenderInterface;
class InputInterface;
struct IEvent;
struct IEventKeyPress;

struct ImmutableLevelData
{
    ImmutableLevelData() = default;
    ~ImmutableLevelData() = default;

    // Core information
    int tilePixelSize = -1;
    Vector2i levelSize;
    int tileCount = -1;

    // Rendering information
    TileRenderMap tileRenderMap;
    std::vector<std::vector<TileRef>> mapRendering;

    // Map information


    // Game logic
    Vector2i entranceTile;
    Vector2i exitTile;

    // Game actors

};

class LevelContext
{

};

using LevelContextPtr = std::shared_ptr<LevelContext>;

class Level
{
public:

    explicit Level(ImmutableLevelData&& imd, LevelContextPtr ctx);
    virtual ~Level() = default;

    bool input(IEvent &evt);
    bool handleKeyInput( IEventKeyPress& evt );

    void update(uint32_t ticks, InputInterface& iinter, RenderInterface &rInter);

private:

    void updateCamera(uint32_t ticks, InputInterface& iinter, RenderInterface &rInter);
    void render(uint32_t ticks, InputInterface& iinter, RenderInterface &rInter);
    void renderTiles(uint32_t ticks, RenderInterface &rInter);

private:

    ECS m_ecs;
    const ImmutableLevelData m_imData;
    LevelContextPtr m_ctx;

    EntityFactory m_entityFactory;

    std::unique_ptr<Player> m_player;

};
using LevelPtr = std::unique_ptr<Level>;
