#pragma once

#include <cstdint>
#include <memory>
#include <functional>

#include <engine/InputInterface.h>
#include <engine/Entity.h>
#include <game/Grid.h>
#include <game/Action.h>


// Forward definitions
class Level;
class RenderInterface;
class SingleTileTargeting;
class SingleEntityTargeting;

namespace UI
{
    class Manager;
    class Element;
}


class LevelController
{
public:

    LevelController(Level* level, UI::Manager* ui);
    virtual ~LevelController() = default;

    bool input(IEvent &evt);
    void update(std::uint32_t ticks, InputInterface& iinter, RenderInterface &rInter);

    void onEnter();
    void onExit();

    bool hasNextController() const;
    std::shared_ptr<LevelController> getNextController();
    bool shouldPopController() const;
    void popController();

    void pushActionController( EntityRef ref, GameAction const& action );
    
    template <typename T, typename... Args>
    void pushController( Args&&... args)
    {
        static_assert( std::is_base_of_v<LevelController, T> );

        auto ptr = std::make_shared<T>( std::forward<Args>(args)... );
        m_nextController = std::static_pointer_cast<LevelController>(ptr);
    }
    
    template <typename T, typename... Args>
    void replaceController( Args&&... args)
    {
        pushController<T>( std::forward<Args>(args)... );
        m_shouldPopController = true;
    }

protected:

    void addKeybinding( SDL_Keycode key, std::function<void()> const& callback );
    void removeKeybinding( SDL_Keycode key );

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
    UI::Manager* m_ui;
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

    explicit DefaultLController(Level* level, UI::Manager* ui);
    ~DefaultLController() override = default;


protected:
    void onExitImpl() override;
    void updateImpl(std::uint32_t ticks, InputInterface &iinter, RenderInterface &rInter) override;

private:
    bool onMouseMove(IEventMouseMove evt) override;
    bool onMouseDown(IEventMouseDown evt) override;
    bool onKeyDown(IEventKeyPress evt) override;
    void onHoveredTileChange(Vector2i prev, Vector2i curr) override;


    EntityRef m_hoveredEntity;
};



// The state in which a player character has been selected. Show information about the PC, and highlight the moves
// that are available to take.
class PlayerSelectedController : public LevelController
{
public:

    PlayerSelectedController(Level*, UI::Manager* ui, EntityRef entity);
    ~PlayerSelectedController() override = default;

protected:
    void onEnterImpl() override;
    void onExitImpl() override;
    void updateImpl(std::uint32_t ticks, InputInterface &iinter, RenderInterface &rInter) override;

private:
    bool onMouseDown(IEventMouseDown evt) override;
    bool onKeyDown(IEventKeyPress evt) override;
    void onHoveredTileChange(Vector2i prev, Vector2i curr) override;


    EntityRef m_entity;
    std::shared_ptr<UI::Element> m_defaultMoveHighlight;
    std::shared_ptr<UI::Element> m_defaultAttackHighlight;
    
    GameAction m_defMoveAction;
    GameAction m_defAttackAction;
    
    std::shared_ptr<SingleTileTargeting> m_defMoveTargeting;
    std::shared_ptr<SingleEntityTargeting> m_defAttackTargeting;
    
    GridRegion m_defMoveTiles;
    GridRegion m_defAttackTiles;
};



// A PC and an action with a single tile target have been selected. Highlight the possible targets and handle
// target selection
class ActionControllerSingleTile : public LevelController
{
public:
    ActionControllerSingleTile(Level* level, UI::Manager* ui, EntityRef ref, GameAction const& action);
    ~ActionControllerSingleTile() override = default;

protected:
    bool onMouseDown(IEventMouseDown evt) override;
    void onExitImpl() override;
    void updateImpl(std::uint32_t ticks, InputInterface &iinter, RenderInterface &rInter) override;

    void onHoveredTileChange(Vector2i prev, Vector2i curr) override;

private:
    GameAction m_action;
    std::shared_ptr<SingleTileTargeting> m_targeting;

    // All the tiles we could move to
    std::shared_ptr<UI::Element> m_tileHighlight;

    // The path that would be taken to get to the highlighted tile
    std::shared_ptr<UI::Element> m_pathHighlight;
    std::vector<Vector2i> m_tilePath;

    // Our entity and the location of that entity
    EntityRef m_entity;
    Vector2i m_origin;
};



class ActionControllerSingleEntity : public LevelController
{
public:
    ActionControllerSingleEntity(Level* level, UI::Manager* ui, EntityRef ref, GameAction const& action);
    ~ActionControllerSingleEntity() override = default;

protected:
    bool onMouseDown(IEventMouseDown evt) override;

    void onEnterImpl() override;
    void onExitImpl() override;
    void updateImpl(std::uint32_t ticks, InputInterface &iinter, RenderInterface &rInter) override;

    void onHoveredTileChange(Vector2i prev, Vector2i curr) override;

private:
    GameAction m_action;
    std::shared_ptr<SingleEntityTargeting> m_targeting;

    // The possible targets
    std::shared_ptr<UI::Element> m_tileHighlight;

    // Our entity and the location of that entity
    EntityRef m_entity;
};