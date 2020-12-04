#pragma once

#include <cstdint>
#include <memory>

#include <engine/InputInterface.h>
#include <controllers/Controller.h>
#include <engine/Entity.h>
#include <game/Grid.h>

// Forward definitions
class Level;
class RenderInterface;
namespace UI { class Element; }

class LevelController : public Controller
{
public:
    explicit LevelController( Level* level );
    ~LevelController() override = default;

    bool input(IEvent &evt) override;

protected:
    virtual void onHoveredTileChange(Vector2i prev, Vector2i curr);
    bool scrollLevel(std::uint32_t ticks, InputInterface &iinter);

    Level* m_level;
    Vector2i m_lastHoveredTile;
};



class DefaultLController : public LevelController
{

public:
    explicit DefaultLController(Level *level);
    ~DefaultLController() override = default;

    void update(std::uint32_t ticks, InputInterface &iinter, RenderInterface &rInter) override;

protected:
    void onExitSelf() override;

private:
    bool onMouseMove(IEventMouseMove evt) override;
    bool onMouseDown(IEventMouseDown evt) override;
    bool onKeyDown(IEventKeyPress evt) override;
    void onHoveredTileChange(Vector2i prev, Vector2i curr) override;

    std::shared_ptr<UI::Element> m_tileHighlight;
};




class EntityMoveController : public LevelController
{

public:
    EntityMoveController(Level*, EntityRef entity);
    void update(std::uint32_t ticks, InputInterface &iinter, RenderInterface &rInter) override;

protected:
    void onExitSelf() override;

private:
    bool onMouseDown(IEventMouseDown evt) override;
    bool onKeyDown(IEventKeyPress evt) override;
    void onHoveredTileChange(Vector2i prev, Vector2i curr) override;

    std::shared_ptr<UI::Element> m_tileHighlight;
    std::shared_ptr<UI::Element> m_pathHighlight;
    std::vector<Vector2i> m_tilePath;
    EntityRef m_entity;
    Vector2i m_origin;
    PathMap m_pathMap;
};
