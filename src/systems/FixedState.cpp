#include <systems/FixedState.h>
#include <game/Level.h>
#include <components/FixedState.h>
#include <components/Render.h>
#include <components/Description.h>
#include <utils/Assert.h>

Systems::FixedState::FixedState(Level *parent) : System(parent)
{
    m_level->events().subscribe<GEvents::FixedStateChange>( this );
}

void Systems::FixedState::accept(GEvents::FixedStateChange *evt)
{
    auto compFs = m_level->tryGetComponent<Components::FixedState>(evt->ent);
    Assert(!!compFs);
    compFs->current = evt->state;

    auto cmpRender = m_level->tryGetComponent<Components::Render>(evt->ent);
    if ( cmpRender )
    {
        cmpRender->current = evt->state;
    }

    auto cmpDesc = m_level->tryGetComponent<Components::Description>(evt->ent);
    if ( cmpDesc )
    {
        cmpDesc->current = evt->state;
    }
}