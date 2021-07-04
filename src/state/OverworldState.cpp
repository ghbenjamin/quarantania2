#include <state/OverworldState.h>

#include <memory>

#include <game/Overworld.h>
#include <ui/lib/Manager.h>
#include <game/OverworldFactory.h>
#include <ui/overworld/LocationView.h>

OverworldState::OverworldState()
{
    m_ui = std::make_unique<UI::Manager>();

    OverworldFactory factory;
    m_overworld = factory.createOverworld();
    
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
    auto turnOrderContainer = m_ui->createElement<UI::LocationView>(nullptr, m_overworld.get());
    m_ui->alignElementToWindow( turnOrderContainer, UI::Alignment::CentreLeft, {20, 0} );
}
