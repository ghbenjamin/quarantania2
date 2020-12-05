#pragma once

#include <cstdint>
#include <functional>
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

    void addKeybinding( SDL_KeyCode key, std::function<void()> const& callback );

    Level* m_level;
    Vector2i m_lastHoveredTile;
    std::unordered_map<SDL_Keycode, std::function<void()>> m_keybinds;
};


// The default state the level controller is in - nothing selected, nothing highlighted, nothing happening.
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
};



// The state in which an entity has been selected to move it - handle displaying which tiles can be moved to
// and associated actions
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

    // All the tiles we could move to
    std::shared_ptr<UI::Element> m_tileHighlight;

    // The path that would be taken to get to the highlighted tile
    std::shared_ptr<UI::Element> m_pathHighlight;
    std::vector<Vector2i> m_tilePath;

    // Our entity and the location of that entity
    EntityRef m_entity;
    Vector2i m_origin;

    // Cached pathfinding data
    PathMap m_pathMap;
};
