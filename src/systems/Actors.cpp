#include <systems/Actors.h>
#include <game/Component.h>
#include <game/Level.h>

Systems::Actors::Actors(Level *parent) : System(parent)
{
    m_level->events().subscribe<GEvents::GameTick>( this );
}

void Systems::Actors::accept(GEvents::GameTick *evt)
{
    // Get all the actors
    auto actors = m_level->entitiesWith<Components::Actor>();

//    // Put them in initiative (turn) order
//    std::sort(actors.begin(), actors.end(), [&](auto const& lhs, auto const& rhs) {
//        return lhs.get<>()->initiative < rhs.initiative;
//    });

    for ( auto& [curr] : actors )
    {
        curr->currentEnergy += curr->energyPerTick;
        if ( curr->currentEnergy >= curr->maxEnergy )
        {
            if ( curr->nextAction )
            {
                // If we have an action queued, try to perform it
                auto actionRes = curr->nextAction->perform();

                if (actionRes.succeeded)
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
