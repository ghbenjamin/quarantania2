#include <state/OverworldState.h>

#include <memory>

#include <game/Overworld.h>
#include <ui/lib/Manager.h>
#include <game/OverworldFactory.h>

OverworldState::OverworldState()
{
    m_ui = std::make_unique<UI::Manager>();

    OverworldFactory factory;
    m_overworld = factory.createOverworld();
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
}
