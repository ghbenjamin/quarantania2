#include <state/OverworldState.h>

#include <memory>

#include <game/Overworld.h>
#include <ui/lib/Manager.h>
#include <ui/lib/ScrollHolder.h>
#include <game/OverworldFactory.h>
#include <ui/shared/OverworldView.h>
#include <state/LevelState.h>
#include <game/RunState.h>

OverworldState::OverworldState(std::shared_ptr<RunState> runState)
    : m_runState(runState)
{
    OverworldFactory factory;
    
    m_overworld = factory.createOverworld( runState );
    runState->overworld = m_overworld;
    m_eventSub = std::make_unique<OverworldStateEventSub>( this );
    
    setupUI();
}

bool OverworldState::inputImpl( IEvent &evt )
{
    return m_overworld->input( evt );
}

void OverworldState::updateImpl( uint32_t ticks, InputInterface &iinter, RenderInterface &rInter )
{
    m_overworld->update( ticks, iinter, rInter );
}

void OverworldState::setupUI()
{
    auto locationViewHolder = m_ui->createElement<UI::ScrollHolder>( nullptr );
    locationViewHolder->setPreferredContentSize({1, 700});

    m_ui->alignElementToWindow( locationViewHolder, UI::Alignment::Centre, {0, 0} );

    auto locationView = m_ui->createElement<UI::OverworldView>(locationViewHolder.get(), m_overworld.get(), false);
}

void OverworldState::startLevel()
{
    replaceNextState<LevelState>( m_runState, "arena" );
}

Overworld *OverworldState::overworld()
{
    return m_overworld.get();
}

OverworldStateEventSub::OverworldStateEventSub( OverworldState *parent )
    : m_parent(parent)
{
    parent->overworld()->events().subscribe<GameEvents::OverworldLocationSelect>( this );
}

void OverworldStateEventSub::operator()( GameEvents::OverworldLocationSelect &evt )
{
    m_parent->startLevel();
}
