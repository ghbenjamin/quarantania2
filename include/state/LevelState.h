#pragma once

#include <state/GameState.h>
#include <game/GameEvent.h>
#include <game/GameEventDefs.h>
#include <utils/Colour.h>

// Forward definitions
class Level;
class RunState;
class LevelController;
class LSUISystem;


class LevelState : public GameState
{
public:
    LevelState(std::shared_ptr<RunState> const& runState);
    ~LevelState() override = default;

    Level* level();

protected:
    bool inputImpl(IEvent &evt) override;
    void updateImpl(uint32_t ticks, InputInterface& iinter, RenderInterface &rInter) override;
    
private:
    void setupUI( );
    
    std::shared_ptr<RunState> m_runState;
    std::unique_ptr<Level> m_level;
    std::unique_ptr<LSUISystem> m_uiWatcher;
    std::vector<std::shared_ptr<LevelController>> m_controllers;
};



class LSUISystem : public GameEventSub<LSUISystem>
{
public:
    explicit LSUISystem(LevelState* parent);
    ~LSUISystem() override = default;
    
    void operator()(GameEvents::LevelReady& evt);
    void operator()(GameEvents::EntityMove& evt);
    void operator()(GameEvents::ItemPickup& evt);
    void operator()(GameEvents::ItemDrop& evt);
    void operator()(GameEvents::ItemEquip& evt);
    void operator()(GameEvents::ItemUnequip& evt);
    void operator()(GameEvents::TurnChange& evt);
    void operator()(GameEvents::RoundChange& evt);
    void operator()(GameEvents::CombatMeleeAttack& evt);
    void operator()(GameEvents::EntityDamage& evt);
    void operator()(GameEvents::ControllerEntitySelected& evt);
    void operator()(GameEvents::ControllerEntityHovered& evt);
    void operator()(GameEvents::EntityAction& evt);
    void operator()(GameEvents::CombatMissedAttack& evt);
    void operator()(GameEvents::CombatAttackSucceeded& evt);
    void operator()(GameEvents::EntityDeath& evt);
    
    template <typename T>
    void operator()(T&& t)
    {}


private:
    
    void refreshPlayerInventory();
    void refreshPlayerEquipView();
    void refreshPartyStatus();
    
    void pushLogLine( std::string const& line, Colour const& colour = Colour::White );
    
    LevelState* m_parent;
};