#include <systems/Message.h>
#include <game/Level.h>

Systems::Message::Message(Level *parent)
        : System(parent)
{
    m_level->events().subscribe<GEvents::LevelReady>( this );
    m_level->events().subscribe<GEvents::EntityMove>( this );
    m_level->events().subscribe<GEvents::EntityOpenClose>( this );
}

void Systems::Message::accept(GEvents::LevelReady *evt)
{

}

void Systems::Message::accept(GEvents::EntityMove *evt)
{

}

void Systems::Message::accept(GEvents::EntityOpenClose *evt)
{

}

