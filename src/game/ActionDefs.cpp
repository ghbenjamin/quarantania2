#include <game/ActionDefs.h>

#include <optional>

#include <game/Level.h>
#include <components/All.h>
#include <utils/Assert.h>
#include <game/GameEventDefs.h>

bool StepAction::doAction() const
{
    auto tilePos = m_level->ecs().getComponents<PositionComponent>(m_actor);
    m_level->events().broadcast<GameEvents::EntityMove>(m_actor, tilePos->position, m_tile );

    return true;
}

bool StepAction::canTryAction() const
{
    auto tilePos = m_level->ecs().getComponents<PositionComponent>(m_actor);

    if ( !GridUtils::isAdjacent(tilePos->position, m_tile ) )
    {
        return false;
    }

    else if ( m_level->grid().pass().valueAt(m_tile) == Passibility::Impassable )
    {
        return false;
    }

    return true;
}

const char *StepAction::description() const
{
    return "Move";
}


bool OpenAction::doAction() const
{
    m_level->events().broadcast<GameEvents::EntityOpenClose>(m_subject, true );
    return true;
}

bool OpenAction::canTryAction() const
{
    auto openable = m_level->ecs().getComponents<OpenableComponent>(m_subject);
    if (!openable) return false;

    if (openable->isOpen) return false;

    if (m_level->squaredEntityDistance( m_actor, m_subject ) > 2) return false;

    return true;
}

const char *OpenAction::description() const
{
    return "Open";
}

bool CloseAction::doAction() const
{
    m_level->events().broadcast<GameEvents::EntityOpenClose>(m_subject, false );
    return true;
}

bool CloseAction::canTryAction() const
{
    auto openable = m_level->ecs().getComponents<OpenableComponent>(m_subject);
    if (!openable) return false;

    if (!openable->isOpen) return false;

    if (m_level->squaredEntityDistance( m_actor, m_subject ) > 2) return false;

    return true;
}

const char *CloseAction::description() const
{
    return "Close";
}

const char *UnlockAction::description() const
{
    return "Unlock";
}

bool UnlockAction::canTryAction() const
{
    auto lockable = m_level->ecs().getComponents<LockableComponent>(m_subject);
    if (!lockable) return false;
    return lockable->isLocked;
}

bool UnlockAction::doAction() const
{
    auto lockable = m_level->ecs().getComponents<LockableComponent>(m_subject);
    return false;
}

const char *ExitLevelAction::description() const
{
    return "Descend";
}

bool ExitLevelAction::canTryAction() const
{
    if (m_level->squaredEntityDistance( m_actor, m_subject ) > 2) return false;

    return true;
}

bool ExitLevelAction::doAction() const
{
    // Exit the level!
    m_level->setComplete();
    return true;
}

const char *MeleeAttackAction::description() const
{
    return "Attack";
}

bool MeleeAttackAction::canTryAction() const
{
    if ( m_actor == m_subject ) return false;

    auto actorC = m_level->ecs().getComponents<ActorComponent>( m_actor );
    auto weapon = actorC->actor.getActiveWeapon();

    if ( !weapon ) return false;

    // TODO Account for weapons with variable reach
    if (m_level->squaredEntityDistance( m_actor, m_subject ) > 2) return false;

    return true;
}

bool MeleeAttackAction::doAction() const
{
    auto actorC = m_level->ecs().getComponents<ActorComponent>( m_actor );
    auto weapon = actorC->actor.getActiveWeapon();

    m_level->events().broadcast<GameEvents::MeleeAttack>(m_actor, m_subject, weapon );
    return true;
}

const char *PickUpItemAction::description() const
{
    return "Pick up";
}

bool PickUpItemAction::canTryAction() const
{
    if ( !m_level->ecs().entityHas<ContainerComponent>(m_actor) ) return false;

    return true;
}

bool PickUpItemAction::doAction() const
{
    auto container = m_level->ecs().getComponents<ContainerComponent>( m_actor );

    if ( (int)container->items.size() + 1 > container->maxItems ) return false;

    m_level->events().broadcast<GameEvents::ItemPickup>(m_actor, m_subject );
    return true;
}


DropItemAction::DropItemAction(Level *level, EntityRef actor, std::shared_ptr<Item> item)
        : Action(level), m_actor(actor), m_item(item)
{

}

const char *DropItemAction::description() const
{
    return "Drop";
}

bool DropItemAction::canTryAction() const
{
    return true;
}

bool DropItemAction::doAction() const
{
    auto container = m_level->ecs().getComponents<ContainerComponent>( m_actor );
    m_level->events().broadcast<GameEvents::ItemDrop>(m_actor, m_item );

    return true;
}

EquipItemAction::EquipItemAction(Level *level, EntityRef actor, std::shared_ptr<Item> item)
        : Action(level), m_actor(actor), m_item(item)
{
}

const char *EquipItemAction::description() const
{
    return "Equip";
}

bool EquipItemAction::canTryAction() const
{
    return true;
}

bool EquipItemAction::doAction() const
{
    auto containerC = m_level->ecs().getComponents<ContainerComponent>( m_actor );
    auto actorC = m_level->ecs().getComponents<ActorComponent>( m_actor );
    auto slot = m_item->getEquipSlot();

    if ( slot == EquipSlot::None )
    {
        AssertAlwaysMsg( "Equipping an item with no equip slots marked" );
    }

    m_level->events().broadcast<GameEvents::ItemEquip>(m_actor, m_item, slot );

    return true;
}

UnequipItemAction::UnequipItemAction(Level *level, EntityRef actor, EquipSlot slot)
        : Action(level), m_actor(actor), m_slot(slot)
{
}

const char *UnequipItemAction::description() const
{
    return "Unquip";
}

bool UnequipItemAction::canTryAction() const
{
    return true;
}

bool UnequipItemAction::doAction() const
{
    m_level->events().broadcast<GameEvents::ItemUnequip>(m_actor, m_slot );
    return true;
}
