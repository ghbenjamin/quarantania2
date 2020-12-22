#pragma once

#include <cstdint>
#include <memory>
#include <functional>

#include <engine/InputInterface.h>
#include <engine/Entity.h>
#include <game/Grid.h>


// Forward definitions
class Level;
class RenderInterface;
namespace UI { class Element; }
class Action;

// Forward definitions
class RenderInterface;

class LevelController
{
public:

    LevelController(Level* level);
    virtual ~LevelController() = default;

    bool input(IEvent &evt);
    void update(std::uint32_t ticks, InputInterface& iinter, RenderInterface &rInter);

    void onEnter();
    void onExit();

    bool hasNextController() const;
    std::shared_ptr<LevelController> getNextController();
    bool shouldPopController() const;
    void popController();

    void pushActionController( EntityRef ref, std::shared_ptr<Action> const& action );


protected:

    template <typename T, typename... Args>
    void setNextController(Args... args)
    {
        static_assert( std::is_base_of_v<LevelController, T> );

        auto ptr = std::make_shared<T>( std::forward<Args>(args)... );
        m_nextController = std::static_pointer_cast<LevelController>(ptr);
    }

    void addKeybinding( SDL_Keycode key, std::function<void()> const& callback );

    virtual void onEnterImpl();
    virtual void onExitImpl();
    virtual bool inputImpl(IEvent &evt);
    virtual void updateImpl(std::uint32_t ticks, InputInterface& iinter, RenderInterface &rInter);

    virtual bool onMouseMove(IEventMouseMove evt);
    virtual bool onMouseDown(IEventMouseDown evt);
    virtual bool onMouseUp(IEventMouseUp evt);
    virtual bool onKeyDown(IEventKeyPress evt);

    virtual void onHoveredTileChange(Vector2i prev, Vector2i curr);
    bool scrollLevel(std::uint32_t ticks, InputInterface &iinter);


    Level* m_level;
    Vector2i m_lastHoveredTile;

private:
    std::shared_ptr<LevelController> m_nextController;
    bool m_shouldPopController;
    std::unordered_map<SDL_Keycode, std::function<void()>> m_keybinds;
};


// The default state the level controller is in - nothing selected, nothing highlighted, nothing happening.
class DefaultLController : public LevelController
{
public:

    explicit DefaultLController(Level *level);
    ~DefaultLController() override = default;


protected:
    void onExitImpl() override;
    void updateImpl(std::uint32_t ticks, InputInterface &iinter, RenderInterface &rInter) override;

private:
    bool onMouseMove(IEventMouseMove evt) override;
    bool onMouseDown(IEventMouseDown evt) override;
    bool onKeyDown(IEventKeyPress evt) override;
    void onHoveredTileChange(Vector2i prev, Vector2i curr) override;
};



// The state in which a player character has been selected. Show information about the PC, and highlight the moves
// that are available to take.
class PlayerSelectedController : public LevelController
{
public:

    PlayerSelectedController(Level*, EntityRef entity);
    ~PlayerSelectedController() override = default;

protected:
    void onExitImpl() override;
    void updateImpl(std::uint32_t ticks, InputInterface &iinter, RenderInterface &rInter) override;

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

class ActionController: public LevelController
{
public:
    ActionController(Level* level, EntityRef ref, std::shared_ptr<Action> const& action);
    ~ActionController() override = default;

private:
    EntityRef m_entity;
    std::shared_ptr<Action> m_action;
};
