#include <systems/ActorSystem.h>
#include <game/Level.h>
#include <game/Damage.h>
#include <components/ActorComponent.h>

ActorSystem::ActorSystem(Level *parent) : System(parent)
{
    m_level->events().subscribe<GEvents::GameTick>( this );
    m_level->events().subscribe<GEvents::EntityDeath>( this );
    m_level->events().subscribe<GEvents::EntityDamage>( this );
}

void ActorSystem::accept(GEvents::GameTick *evt)
{
    // Get all the actors
    auto actors = m_level->entitiesWith<ActorComponent>();

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

void ActorSystem::accept(GEvents::EntityDeath *evt)
{
    m_level->addTextLogMessage( fmt::format( "{} was struck down",
            m_level->getDescriptionForEnt(evt->actor)
    ));

    m_level->deleteEntityDelayed( evt->actor );
}

void ActorSystem::accept(GEvents::EntityDamage *evt)
{
    int finalDamage = evt->damage->total;
    auto actorC = m_level->getComponents<ActorComponent>( evt->target );

    // TODO Resistances and immunities

    if ( finalDamage < 1 || evt->damage->type == DamageType::Nonlethal )
    {
        if ( finalDamage < 1 )
        {
            finalDamage = 1;
        }

        actorC->nonLethalDamage += finalDamage;

        if (actorC->nonLethalDamage == actorC->currentHP )
        {
            // Staggered
        }
        else if (actorC->nonLethalDamage > actorC->currentHP )
        {
            // Unconsious
        }
    }
    else
    {
        // Lethal damage

        actorC->currentHP -= finalDamage;

        if (actorC->currentHP == 0 )
        {
            // Disabled
        }
        else if (actorC->currentHP < 0 && actorC->currentHP > -actorC->getConMod() )
        {
            // Dying
        }
        else if (actorC->currentHP < -actorC->getConMod() )
        {
            // Dead
            m_level->events().broadcast<GEvents::EntityDeath>( evt->target );
        }
    }
}
