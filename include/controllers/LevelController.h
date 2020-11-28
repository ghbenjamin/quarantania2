#pragma once

#include <cstdint>
#include <memory>

#include <engine/InputInterface.h>
#include <controllers/Controller.h>

class Level;
class RenderInterface;

class LevelController : public Controller
{
public:
    explicit LevelController( Level* level );
    virtual ~LevelController() = default;

    virtual bool input(IEvent &evt);

protected:
    virtual void onHoveredTileChange(Vector2i prev, Vector2i curr);
    void scrollLevel(std::uint32_t ticks, InputInterface &iinter);

    Level* m_level;
    Vector2i m_lastHoveredTile;
};

using LevelControllerPtr = std::unique_ptr<LevelController>;




class DefaultLController : public LevelController
{

public:
    explicit DefaultLController(Level *level);
    ~DefaultLController() override = default;

    void update(std::uint32_t ticks, InputInterface &iinter, RenderInterface &rInter) override;

private:

    bool onMouseMove(IEventMouseMove evt) override;
    bool onMouseDown(IEventMouseDown evt) override;
    bool onKeyDown(IEventKeyPress evt) override;
    void onHoveredTileChange(Vector2i prev, Vector2i curr) override;
};

class EntityMoveController : public DefaultLController
{

public:
    void update(std::uint32_t ticks, InputInterface &iinter, RenderInterface &rInter) override;

private:
    bool onMouseDown(IEventMouseDown evt) override;
    bool onKeyDown(IEventKeyPress evt) override;
    void onHoveredTileChange(Vector2i prev, Vector2i curr) override;


};
