#pragma once

#include <cstdint>
#include <memory>
#include <game/InputInterface.h>

class Level;
struct IEvent;
class InputInterface;
class RenderInterface;

class LevelController
{
public:
    explicit LevelController( Level* level );
    virtual ~LevelController() = default;

    bool input(IEvent &evt);
    virtual void update(std::uint32_t ticks, InputInterface& iinter, RenderInterface &rInter);

private:

    virtual bool onMouseMove(IEventMouseMove evt);
    virtual bool onMouseDown(IEventMouseDown evt);
    virtual bool onMouseUp(IEventMouseUp evt);
    virtual bool onKeyDown(IEventKeyPress evt);

    virtual void onHoveredTileChange(Vector2i prev, Vector2i curr);

protected:
    Level* m_level;

private:
    Vector2i m_lastHoveredTile;

};

using LevelControllerPtr = std::unique_ptr<LevelController>;