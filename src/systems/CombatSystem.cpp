#include <systems/CombatSystem.h>
#include <game/Level.h>
#include <components/ActorComponent.h>
#include <game/Damage.h>

CombatSystem::CombatSystem(Level *parent)
        : System(parent)
{
    m_level->events().subscribe<GameEvents::MeleeAttack>(this );
}

void CombatSystem::accept(GameEvents::MeleeAttack *evt)
{
}
