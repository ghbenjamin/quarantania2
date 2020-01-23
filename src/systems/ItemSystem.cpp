#include <systems/ItemSystem.h>
#include <components/ContainerComponent.h>
#include <components/ItemComponent.h>
#include <game/Level.h>

ItemSystem::ItemSystem(Level *parent)
        : System(parent)
{
    m_level->events().subscribe<GEvents::ItemPickup>( this );

}

void ItemSystem::accept(GEvents::ItemPickup *evt)
{
    auto cont = m_level->getComponents<ContainerComponent>(evt->actor);
    auto item = m_level->getComponents<ItemComponent>(evt->item);

    // TODO: Actually pick it up, don't just evaporate it
    m_level->deleteEntity( evt->item );
}
