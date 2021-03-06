#include <state/LevelState.h>
#include <game/Level.h>
#include <game/FixedLevelFactory.h>
#include <components/ActorComponent.h>
#include <game/LevelController.h>
#include <ui/lib/Manager.h>
#include <ui/level/PlayerStatusView.h>
#include <ui/level/ActionPopupMenu.h>
#include <ui/level/CreatureEquipView.h>
#include <ui/level/ContainerView.h>
#include <ui/level/EntityInformationView.h>
#include <ui/level/BetterTextLog.h>
#include <ui/level/Composites.h>

LevelState::LevelState(std::shared_ptr<LevelContext> const& ctx, std::shared_ptr<RunState> const& runState )
: m_levelCtx(ctx)
{
    TiledMapLoader loader;
    TiledMap tm = loader.load( "../resource/maps/arena.json" );

    FixedLevelFactory ffactory(&tm, m_levelCtx, runState);
    m_level = ffactory.createLevel();
    m_ui = std::make_unique<UI::Manager>();
    setupUI();
    m_uiWatcher = std::make_unique<LSUISystem>( m_level.get(), m_ui.get() );
    m_controllers = { std::make_shared<DefaultLController>( m_level.get(), m_ui.get() ) };
}

bool LevelState::input(IEvent &evt)
{
    if ( m_ui->input(evt))
    {
        return true;
    }
    else if ( m_level->input(evt) )
    {
        return true;
    }
    else
    {
        return m_controllers.back()->input(evt);
    }
}

void LevelState::update(uint32_t ticks, InputInterface& iinter, RenderInterface &rInterface)
{
    m_level->update(ticks, iinter, rInterface);
    
    if ( m_controllers.back()->hasNextController() )
    {
        auto next = m_controllers.back()->getNextController();
        bool shouldPop = m_controllers.back()->shouldPopController();

        m_controllers.back()->onExit();

        if (shouldPop)
        {
            m_controllers.pop_back();
        }

        m_controllers.push_back( next );
        m_controllers.back()->onEnter();
    }

    else if ( m_controllers.back()->shouldPopController() )
    {
        m_controllers.back()->onExit();
        m_controllers.pop_back();
        m_controllers.back()->onEnter();
    }

    m_controllers.back()->update(ticks, iinter, rInterface);
    
    // Render the GUI
    m_ui->update(ticks, iinter, rInterface);

    if ( m_level->getLevelExitStatus() != LevelExitStatus::None )
    {
        // TODO: Serialize level state
    
    
        switch ( m_level->getLevelExitStatus() )
        {
            case LevelExitStatus::Completed:
                requestPopState();
                break;
            case LevelExitStatus::MainMenu:
                requestExit();
                break;
            case LevelExitStatus::Desktop:
                requestExit();
                break;
            default:
                AssertAlways();
        }
    }
}

void LevelState::setupUI()
{
    // Fixed-in-place UI Elements
    
    // Widget containing the current party and information
    auto turnOrderContainer = m_ui->createElement<UI::PlayerStatusContainer>(nullptr, m_level.get());
    m_ui->alignElementToWindow( turnOrderContainer, UI::Alignment::TopLeft, {20, 20} );
    
    // Widget containing icons representing actions which can be taken
    auto actionMenu = m_ui->createElement<UI::BottomLeftBar>(nullptr, m_level.get());
    m_ui->alignElementToWindow( actionMenu, UI::Alignment::BottomLeft, {20, -20} );
    
    // Widget containing fixed action buttons e.g. end turn & options
    auto bottomMenubar = m_ui->createElement<UI::BottomMenubar>(nullptr, m_level.get());
    m_ui->alignElementToWindow( bottomMenubar, UI::Alignment::BottomCentre, {0, -20} );
    
    // Widget containing the global text log
    auto textLog = m_ui->createElement<UI::BetterTextLog>(nullptr, m_level.get());
    m_ui->alignElementToWindow( textLog, UI::Alignment::BottomRight, {-20, -20} );
    
    
    // Default hidden elements
    
    
    auto entityInfo = m_ui->createElement<UI::EntityInformationView>(nullptr, m_level.get());
    m_ui->alignElementToWindow( entityInfo, UI::Alignment::TopRight, {-20, 20} );
    entityInfo->hide();
    
    auto playerInventory = m_ui->createElement<UI::ContainerView>(nullptr, m_level.get(), Vector2i{6, 2}); // TODO container size from container
    m_ui->alignElementToElement( playerInventory, textLog, UI::Alignment::TopRight, {0, -10} );
    playerInventory->setId("player-inventory");
    playerInventory->hide();
    
    auto equipUi = m_ui->createElement<UI::EquipView>(nullptr, m_level.get());
    m_ui->alignElementToElement( equipUi, playerInventory, UI::Alignment::TopRight, {0, -10} );
    equipUi->hide();
}


LSUISystem::LSUISystem(Level *level, UI::Manager* ui)
    : m_level(level), m_ui(ui)
{
    m_level->events().subscribe<GameEvents::LevelReady>(this, GEventTiming::After );
    m_level->events().subscribe<GameEvents::EntityMove>(this, GEventTiming::After );
    m_level->events().subscribe<GameEvents::ItemPickup>(this, GEventTiming::After );
    m_level->events().subscribe<GameEvents::ItemDrop>(this, GEventTiming::After );
    m_level->events().subscribe<GameEvents::ItemEquip>(this, GEventTiming::After );
    m_level->events().subscribe<GameEvents::ItemUnequip>(this, GEventTiming::After );
    m_level->events().subscribe<GameEvents::TurnChange>(this, GEventTiming::After );
    m_level->events().subscribe<GameEvents::RoundChange>(this, GEventTiming::After );
    m_level->events().subscribe<GameEvents::ControllerEntitySelected>(this, GEventTiming::After );
    m_level->events().subscribe<GameEvents::ControllerEntityHovered>(this, GEventTiming::After );
    m_level->events().subscribe<GameEvents::CombatMeleeAttack>(this, GEventTiming::After );
    m_level->events().subscribe<GameEvents::EntityAction>(this, GEventTiming::After );
    m_level->events().subscribe<GameEvents::CombatAttackSucceeded>(this, GEventTiming::After );
    m_level->events().subscribe<GameEvents::CombatMissedAttack>(this, GEventTiming::After );
    m_level->events().subscribe<GameEvents::EntityDeath>(this, GEventTiming::After );
}

void LSUISystem::operator()(GameEvents::LevelReady& evt)
{
    auto playerStatus = m_ui->withId<UI::PlayerStatusContainer>( "player-status-container" );
    playerStatus->reloadEntities();
}


void LSUISystem::operator()(GameEvents::EntityMove& evt)
{

}

void LSUISystem::operator()(GameEvents::ItemPickup& evt)
{

}

void LSUISystem::operator()(GameEvents::ItemDrop& evt)
{

}

void LSUISystem::operator()(GameEvents::ItemEquip& evt)
{
    auto creatureEquip = m_ui->withId<UI::EquipView>( "ui-equip-view" );
    creatureEquip->refresh(evt.actor);
    
    auto creatureInventory = m_ui->withId<UI::ContainerView>( "player-inventory" );
    creatureInventory->refresh(evt.actor);
}

void LSUISystem::operator()(GameEvents::ItemUnequip& evt)
{
    auto creatureEquip = m_ui->withId<UI::EquipView>( "ui-equip-view" );
    creatureEquip->refresh(evt.actor);
    
    auto creatureInventory = m_ui->withId<UI::ContainerView>( "player-inventory" );
    creatureInventory->refresh(evt.actor);
}

void LSUISystem::operator()(GameEvents::TurnChange& evt)
{
    auto playerStatus = m_ui->withId<UI::PlayerStatusContainer>( "player-status-container" );
    playerStatus->refresh();
}

void LSUISystem::operator()(GameEvents::ControllerEntitySelected& evt)
{
    // We've selected an entity - update our UI to match
    auto actionMenu = m_ui->withId<UI::ActionMenu>( "action-menu" );
    actionMenu->closeMenu();
    actionMenu->refresh(evt.entity);
    
    auto creatureEquip = m_ui->withId<UI::EquipView>( "ui-equip-view" );
    creatureEquip->refresh(evt.entity);
    
    auto creatureInventory = m_ui->withId<UI::ContainerView>( "player-inventory" );
    creatureInventory->refresh(evt.entity);
}

void LSUISystem::operator()(GameEvents::CombatMeleeAttack &evt)
{
    auto atkActorC = m_level->ecs().getComponents<ActorComponent>( evt.attacker );
    auto atkDefenderC = m_level->ecs().getComponents<ActorComponent>( evt.defender );
    
    bool attackerHostile = atkActorC->actorType == ActorType::NPC;
    bool defenderHostile = atkDefenderC->actorType == ActorType::NPC;
    
    pushLogLine( fmt::format(
            "{} attacks {}",
            m_level->getDescriptionForEnt( evt.attacker ),
            m_level->getDescriptionForEnt( evt.defender )
    ));
}

void LSUISystem::pushLogLine(const std::string &line, const Colour &colour)
{
    auto textLog = m_ui->withId<UI::BetterTextLog>("main-text-log");
    textLog->addLine(line, colour);
}

void LSUISystem::operator()( GameEvents::EntityDamage &evt )
{
    // TODO: Use real value not unmodified value
    int total = 0;
    for (auto const& d : evt.damage.instances )
    {
        total += d.total;
    }
    
    pushLogLine( fmt::format(
            "{} deals {} damage to {}",
            m_level->getDescriptionForEnt( evt.source ),
            total,
            m_level->getDescriptionForEnt( evt.target )
    ));
}

void LSUISystem::operator()(GameEvents::EntityAction &evt)
{
    refreshPartyStatus();
}

void LSUISystem::operator()(GameEvents::RoundChange &evt)
{
    refreshPartyStatus();
    
    m_ui->displayTransientMessage("Player Turn", 1.0f);
}

void LSUISystem::operator()(GameEvents::CombatMissedAttack &evt)
{
    pushLogLine( fmt::format(
            "{} misses {} ",
            m_level->getDescriptionForEnt( evt.attacker ),
            m_level->getDescriptionForEnt( evt.defender )
    ));
}

void LSUISystem::operator()( GameEvents::CombatAttackSucceeded &evt )
{
    pushLogLine( fmt::format(
            "{} hits {} ",
            m_level->getDescriptionForEnt( evt.attacker ),
            m_level->getDescriptionForEnt( evt.defender )
    ));
}

void LSUISystem::operator()(GameEvents::EntityDeath &evt)
{
    pushLogLine( fmt::format(
            "{} has died! ",
            m_level->getDescriptionForEnt( evt.actor )
    ));
}



void LSUISystem::refreshPlayerInventory()
{

}

void LSUISystem::refreshPlayerEquipView()
{

}

void LSUISystem::refreshPartyStatus()
{
    auto playerStatus = m_ui->withId<UI::PlayerStatusContainer>( "player-status-container" );
    playerStatus->reloadEntities();
}

void LSUISystem::operator()( GameEvents::ControllerEntityHovered &evt )
{
    auto entityInformation = m_ui->withId<UI::EntityInformationView>("entity-information-view");
    entityInformation->refresh(evt.entity);
    
    if (evt.entity == EntityNull)
    {
        entityInformation->hide();
    }
    else
    {
        entityInformation->show();
    }
}

