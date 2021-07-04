#pragma once

#include <state/State.h>
#include <game/GameEvent.h>
#include <game/GameEventDefs.h>
#include <utils/Colour.h>

class Level;
struct PartyData;
struct LevelContext;
class LevelController;
class LSUISystem;
namespace UI { class Manager; }


class LevelState : public GameState
{
public:
    LevelState(std::shared_ptr<LevelContext> const& ctx, PartyData const& pgen );
    ~LevelState() override = default;

    bool input(IEvent &evt) override;
    void update(uint32_t ticks, InputInterface& iinter, RenderInterface &rInter) override;
    
    
private:

    void setupUI( );

    std::unique_ptr<Level> m_level;
    std::unique_ptr<LSUISystem> m_uiWatcher;
    std::unique_ptr<UI::Manager> m_ui;
    std::shared_ptr<LevelContext> m_levelCtx;
    std::vector<std::shared_ptr<LevelController>> m_controllers;
};



class LSUISystem : public GameEventSub<LSUISystem>
{
public:
    explicit LSUISystem(Level *parent, UI::Manager* ui);
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
    
    Level* m_level;
    UI::Manager* m_ui;
};