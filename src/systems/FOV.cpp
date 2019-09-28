#include <systems/FOV.h>
#include <game/Level.h>
#include <graphics/Primatives.h>


Systems::FOV::FOV(Level *parent) : System(parent),
                                   m_fovHidden{ createRectangle({16, 16}, Colour::Black) },
                                   m_fovFog{ createRectangle({16, 16}, Colour::Black.withAlpha(100)) }
{
    m_level->events().subscribe<GEvents::EntityMove>( this );
}

void Systems::FOV::accept(GEvents::EntityMove *evt)
{

}