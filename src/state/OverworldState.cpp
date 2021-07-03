#include <state/OverworldState.h>
#include <game/Overworld.h>

OverworldState::OverworldState()
{
    // TODO Generate overworld
}

bool OverworldState::input( IEvent &evt )
{
    return m_overworld->input( evt );
}

void OverworldState::update( uint32_t ticks, InputInterface &iinter, RenderInterface &rInter )
{
    m_overworld->update( ticks, iinter, rInter );
}
