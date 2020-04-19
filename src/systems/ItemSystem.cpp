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
    m_level->events().subscribe<GEvents::ItemPickup>( this );
    m_level->events().subscribe<GEvents::ItemDrop>( this );
    m_level->events().subscribe<GEvents::ItemEquip>( this );
    m_level->events().subscribe<GEvents::ItemUnequip>( this );
}

void ItemSystem::accept(GEvents::ItemPickup *evt)
{
    auto cont = m_level->getComponents<ContainerComponent>(evt->actor);
    auto item = m_level->getComponents<ItemComponent>(evt->item);

    m_level->addTextLogMessage( fmt::format("{} picks up the {}",
            m_level->getDescriptionForEnt( evt->actor ),
            m_level->getDescriptionForEnt( evt->item  )
    ));

    cont->items.push_back( item->item );
    m_level->deleteEntity( evt->item );
}

void ItemSystem::accept(GEvents::ItemDrop *evt)
{
    auto container = m_level->getComponents<ContainerComponent>( evt->actor );
    auto position = m_level->getComponents<PositionComponent>( evt->actor );

    eraseItemFromContainer(container, evt->item);

    auto itemEnt = m_level->entityFactory().createItem( evt->item, position->position );

    m_level->addTextLogMessage( fmt::format("{} drops the {}",
            m_level->getDescriptionForEnt( evt->actor ),
            m_level->getDescriptionForEnt( itemEnt )
    ));
}

void ItemSystem::accept(GEvents::ItemEquip *evt)
{
    auto containerC = m_level->getComponents<ContainerComponent>( evt->actor );
    auto actorC = m_level->getComponents<ActorComponent>( evt->actor );

    eraseItemFromContainer(containerC, evt->item);

    auto oldItem = actorC->equipItem( evt->slot, evt->item );
    if ( oldItem )
    {
        containerC->items.push_back( oldItem );
    }
}

void ItemSystem::accept(GEvents::ItemUnequip *evt)
{
    auto containerC = m_level->getComponents<ContainerComponent>( evt->actor );
    auto actorC = m_level->getComponents<ActorComponent>( evt->actor );

    auto item = actorC->unequipItem( evt->slot );
    AssertMsg(!!item, "Unequipping empty item slot");

    containerC->items.push_back( item );
}

void ItemSystem::eraseItemFromContainer(std::shared_ptr<ContainerComponent> container, std::shared_ptr<Item> item)
{
    auto it = std::find( container->items.begin(), container->items.end(), item );
    Assert( it != container->items.end() );
    container->items.erase(it);
}
