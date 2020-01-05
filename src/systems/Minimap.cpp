#include <systems/Minimap.h>
#include <game/Level.h>

Systems::Minimap::Minimap(Level *parent)
        : System(parent)
{
    m_level->events().subscribe<GEvents::LevelReady>( this );
}


void Systems::Minimap::accept(GEvents::LevelReady *evt)
{
    m_level->generateMinimapData();
}

