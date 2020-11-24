#include <systems/UISystem.h>
#include <game/Level.h>

UISystem::UISystem(Level *parent)
        : System(parent)
{
    m_level->events().subscribe<GEvents::LevelReady>( this, GEventTiming::After );
    m_level->events().subscribe<GEvents::EntityMove>( this, GEventTiming::After );
    m_level->events().subscribe<GEvents::ItemPickup>( this, GEventTiming::After );
    m_level->events().subscribe<GEvents::ItemDrop>( this, GEventTiming::After );
    m_level->events().subscribe<GEvents::ItemEquip>( this, GEventTiming::After );
    m_level->events().subscribe<GEvents::ItemUnequip>( this, GEventTiming::After );
}

void UISystem::accept(GEvents::LevelReady *evt)
{

}

void UISystem::accept(GEvents::EntityMove *evt)
{

}

void UISystem::accept(GEvents::ItemPickup *evt)
{
    
}

void UISystem::accept(GEvents::ItemDrop *evt)
{
    
}

void UISystem::accept(GEvents::ItemEquip *evt)
{
    
}

void UISystem::accept(GEvents::ItemUnequip *evt)
{
    
}
