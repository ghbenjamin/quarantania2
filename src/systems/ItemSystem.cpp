#include <systems/ItemSystem.h>
#include <components/ContainerComponent.h>
#include <components/ItemComponent.h>
#include <game/Level.h>
#include <utils/Assert.h>
#include <components/PositionComponent.h>
#include <components/ActorComponent.h>

ItemSystem::ItemSystem(Level *parent)
        : System(parent)
{
    m_level->events().subscribe<GameEvents::ItemPickup>(this);
    m_level->events().subscribe<GameEvents::ItemDrop>(this);
    m_level->events().subscribe<GameEvents::ItemEquip>(this);
    m_level->events().subscribe<GameEvents::ItemUnequip>(this);
}

void ItemSystem::operator()(GameEvents::ItemPickup& evt)
{
    auto cont = m_level->ecs().getComponents<ContainerComponent>(evt.actor);
    auto item = m_level->ecs().getComponents<ItemComponent>(evt.item);

    cont->items.push_back( item->item );
    m_level->ecs().deleteEntity( evt.item );
}

void ItemSystem::operator()(GameEvents::ItemDrop& evt)
{
    auto container = m_level->ecs().getComponents<ContainerComponent>( evt.actor );
    auto position = m_level->ecs().getComponents<PositionComponent>( evt.actor );

    eraseItemFromContainer(container, evt.item);

    auto itemEnt = m_level->ecs().entityFactory()
                          .createItem(position->tilePosition, evt.item);
}

void ItemSystem::operator()(GameEvents::ItemEquip& evt)
{
    auto containerC = m_level->ecs().getComponents<ContainerComponent>( evt.actor );
    auto actorC = m_level->ecs().getComponents<ActorComponent>( evt.actor );

    eraseItemFromContainer(containerC, evt.item);

    auto oldItem = actorC->actor.equipItem( evt.slot, evt.item );
    if ( oldItem )
    {
        containerC->items.push_back( oldItem );
    }
}

void ItemSystem::operator()(GameEvents::ItemUnequip& evt)
{
    auto containerC = m_level->ecs().getComponents<ContainerComponent>( evt.actor );
    auto actorC = m_level->ecs().getComponents<ActorComponent>( evt.actor );


    auto item = actorC->actor.unequipItem( evt.slot );
    AssertMsg(!!item, "Unequipping empty item slot");

    containerC->items.push_back( item );
}

void ItemSystem::eraseItemFromContainer(const std::shared_ptr<ContainerComponent>& container, std::shared_ptr<Item> item)
{
    auto it = std::find( container->items.begin(), container->items.end(), item );
    Assert( it != container->items.end() );
    container->items.erase(it);
}
