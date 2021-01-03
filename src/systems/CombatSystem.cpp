#include <systems/CombatSystem.h>
#include <game/Level.h>
#include <components/ActorComponent.h>


CombatSystem::CombatSystem(Level *parent)
        : System(parent)
{
    m_level->events().subscribe<GameEvents::CombatMeleeAttack>(this, GEventTiming::Before);
}

void CombatSystem::operator()(GameEvents::CombatMeleeAttack& evt)
{
    auto aAttacker = m_level->ecs().getComponents<ActorComponent>(evt.attacker)->actor;
    auto aDefender = m_level->ecs().getComponents<ActorComponent>(evt.defender)->actor;
    auto const& weapon = aAttacker.getActiveWeapon();

    int attackRoll = m_level->random().diceRoll(20);
    bool isHit = false;
    bool isCrit = false;

    // Did we hit?

    if ( attackRoll >= weapon.critRange() )
    {
        isHit = true;
        isCrit = true;
    }
    else
    {
        auto defAC = aDefender.getAC();
        if (attackRoll >= defAC)
        {
            isHit = true;
        }
    }

    if (!isHit)
    {
        return;
    }

    // How much damage?
    int damageRoll = m_level->random().diceRoll( weapon.damage() );

    if (isCrit)
    {
        damageRoll *= weapon.critMultiplier();
    }

    Damage dmg;
    DamageInstance dmgInstance{ DamageType::Untyped, DamageSuperType::Physical, damageRoll };
    dmg.instances.push_back( dmgInstance );

    // Actually deal the damage
    acceptDamage( dmg, evt.defender );
}

void CombatSystem::acceptDamage(const Damage& dmg, EntityRef ref)
{
    auto actorC = m_level->ecs().getComponents<ActorComponent>(ref);
    auto& actor = actorC->actor;

    // Decrease our HP for each instance of damage supplied
    // TODO: Immunities, resistances

    for (auto const& instance : dmg.instances )
    {
        auto newNp = actor.getCurrentHp() - instance.total;
        actor.setCurrentHp( newNp );
    }

    // Handle falling unconsious and death

    if ( actor.getCurrentHp() < 0 )
    {
        if ( actorC->actorType == ActorType::PC &&
             actor.getCurrentHp() > -actor.abilityScores().getScore(AbilityScoreType::CON).getValue() )
        {
            // Unconscious! Allow PCs to fall unconsious but not NPCs
        }
        else
        {
            // The entity died
            m_level->events().broadcast<GameEvents::EntityDeath>(ref);
        }
    }
}