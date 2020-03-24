#include <systems/UISystem.h>
#include <game/Level.h>
#include <ui/ContainerView.h>
#include <ui/EquippedItemsView.h>

UISystem::UISystem(Level *parent)
        : System(parent)
{
    m_level->events().subscribe<GEvents::LevelReady>( this );
    m_level->events().subscribe<GEvents::EntityMove>( this, GEventTiming::After );
    m_level->events().subscribe<GEvents::ItemPickup>( this, GEventTiming::After );
    m_level->events().subscribe<GEvents::ItemDrop>( this, GEventTiming::After );
    m_level->events().subscribe<GEvents::ItemEquip>( this, GEventTiming::After );
}

void UISystem::accept(GEvents::LevelReady *evt)
{
    m_level->generateMinimapData();
}

void UISystem::accept(GEvents::EntityMove *evt)
{
    // If the player has moved, the FOV may have changed - regenerate the minimap
    // TODO: Regenerate the minimap on any FOV change, not just player movement
    // TODO: Don't regenerate the minimap for non-FOV altering movement - FOV change event?
    if ( m_level->isPlayer( evt->ent ) )
    {
        m_level->generateMinimapData();
    }
}

void UISystem::accept(GEvents::ItemPickup *evt)
{
    if ( m_level->isPlayer( evt->actor ) )
    {
        m_level->getUIPlayerInventory()->reimportItems();
        m_level->getUIPlayerEquip()->reimportItems();
    }
}

void UISystem::accept(GEvents::ItemDrop *evt)
{
    if ( m_level->isPlayer( evt->actor ) )
    {
        m_level->getUIPlayerInventory()->reimportItems();
        m_level->getUIPlayerEquip()->reimportItems();
    }
}

void UISystem::accept(GEvents::ItemEquip *evt)
{
    if ( m_level->isPlayer( evt->actor ) )
    {
        m_level->getUIPlayerInventory()->reimportItems();
        m_level->getUIPlayerEquip()->reimportItems();
    }
}

