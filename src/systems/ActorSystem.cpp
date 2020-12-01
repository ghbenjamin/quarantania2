#include <systems/ActorSystem.h>
#include <game/Level.h>
#include <game/Damage.h>
#include <components/ActorComponent.h>

ActorSystem::ActorSystem(Level *parent) : System(parent)
{
    m_level->events().subscribe<GameEvents::EntityDeath>(this );
    m_level->events().subscribe<GameEvents::EntityDamage>(this );
}

void ActorSystem::accept(GameEvents::EntityDeath *evt)
{
    m_level->addTextLogMessage( fmt::format( "{} was struck down",
            m_level->getDescriptionForEnt(evt->actor)
    ));

    m_level->ecs().deleteEntityDelayed( evt->actor );
}

void ActorSystem::accept(GameEvents::EntityDamage *evt)
{

}
