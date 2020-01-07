#include <systems/Minimap.h>
#include <game/Level.h>

Systems::Minimap::Minimap(Level *parent)
        : System(parent)
{
    m_level->events().subscribe<GEvents::LevelReady>( this );
    m_level->events().subscribe<GEvents::EntityMove>( this );
}


void Systems::Minimap::accept(GEvents::LevelReady *evt)
{
    m_level->generateMinimapData();
}

void Systems::Minimap::accept(GEvents::EntityMove *evt)
{
    // If the player has moved, the FOV may have changed - regenerate the minimap
    // TODO: Regenerate the minimap on any FOV change, not just player movement
    // TODO: Don't regenerate the minimap for non-FOV altering movement - FOV change event?
    if ( m_level->isPlayer( evt->ent ) )
    {
        m_level->generateMinimapData();
    }
}

