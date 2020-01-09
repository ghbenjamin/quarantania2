#include <systems/Actors.h>
#include <game/Level.h>
#include <components/Actor.h>

Systems::Actors::Actors(Level *parent) : System(parent)
{
    m_level->events().subscribe<GEvents::GameTick>( this );
}

void Systems::Actors::accept(GEvents::GameTick *evt)
{
    // Get all the actors
    auto actors = m_level->entitiesWith<Components::Actor>();

    for ( auto& [curr] : actors )
    {
        curr->currentEnergy += curr->energyPerTick;
        if ( curr->currentEnergy >= curr->maxEnergy )
        {
            if ( curr->nextAction )
            {
                // If we have an action queued, try to perform it
                auto actionRes = curr->nextAction->doAction();

                if (actionRes)
                {
                    // We succeeded! Reset our current energy
                    curr->currentEnergy -= curr->maxEnergy;
                }
                else
                {   // We failed - don't touch our energy
                    curr->currentEnergy = curr->maxEnergy;
                }
            }
            else
            {
                // There's no current action - cap energy at max until there is one
                curr->currentEnergy = curr->maxEnergy;
            }
        }
    }
}
