#pragma once

#include <memory>
#include <vector>

#include <SDL2/SDL.h>

#include <game/ECS.h>
#include <game/Tiles.h>
#include <game/Player.h>
#include <game/LevelData.h>
#include <game/EntityFactory.h>

class RenderInterface;
class InputInterface;
struct IEvent;
struct IEventKeyPress;


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
    bool handleKeyInput(IEventKeyPress& evt);

    void update(uint32_t ticks, InputInterface& iinter, RenderInterface &rInter);

private:

    void updateCamera(uint32_t ticks, InputInterface& iinter, RenderInterface &rInter);
    void render(uint32_t ticks, InputInterface& iinter, RenderInterface &rInter);
    void renderTiles(uint32_t ticks, RenderInterface &rInter);


    // Player input
    void doMovePlayer( SDL_Keycode kcode );

private:

    ECS m_ecs;
    const ImmutableLevelData m_imData;
    LevelContextPtr m_ctx;
    EntityFactory m_entityFactory;
    std::unique_ptr<Player> m_player;

};
using LevelPtr = std::unique_ptr<Level>;
