#include <state/OverworldState.h>

#include <memory>

#include <game/Overworld.h>
#include <ui/lib/Manager.h>
#include <ui/lib/ScrollHolder.h>
#include <game/OverworldFactory.h>
#include <ui/overworld/LocationView.h>

OverworldState::OverworldState()
{
    m_ui = std::make_unique<UI::Manager>();

    OverworldFactory factory;
    m_overworld = factory.createOverworld();
    
    m_eventSub = std::make_unique<OverworldStateEventSub>( m_overworld.get() );
    
    setupUI();
}

bool OverworldState::input( IEvent &evt )
{
    if ( m_ui->input(evt) )
    {
        return true;
    }
    else
    {
        return m_overworld->input( evt );
    }
}

void OverworldState::update( uint32_t ticks, InputInterface &iinter, RenderInterface &rInter )
{
    m_overworld->update( ticks, iinter, rInter );
    m_ui->update(ticks, iinter, rInter);
}

void OverworldState::setupUI()
{
    auto locationViewHolder = m_ui->createElement<UI::ScrollHolder>( nullptr );
    locationViewHolder->setPreferredContentSize({1, 700});

    m_ui->alignElementToWindow( locationViewHolder, UI::Alignment::Centre, {0, 0} );

    auto locationView = m_ui->createElement<UI::LocationView>(locationViewHolder.get(), m_overworld.get());
}

OverworldStateEventSub::OverworldStateEventSub( Overworld *parent )
    : m_overworld(parent)
{
    m_overworld->events().subscribe<GameEvents::OverworldLocationSelect>( this );
}

void OverworldStateEventSub::operator()( GameEvents::OverworldLocationSelect &evt )
{

}
