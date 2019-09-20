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

void Systems::FOV::update(uint32_t ticks, RenderInterface &rInter)
{
    Vector2i curr;
    auto width = m_level->bounds().x();
    for ( int i = 0; i < m_level->tileCount(); i++ )
    {

        auto val = m_level->grid().fov().valueAt( i );
        curr = Vector2i{ i % width, i / width } * 16;

        // Uncomment me once FOV is implemented
//        if ( val == Rules::Visibility::Explored )
//        {
//            rInter.addWorldItem( m_fovFog.renderObject(curr) );
//        }
//        else if ( val == Rules::Visibility::Hidden )
//        {
//            rInter.addWorldItem( m_fovHidden.renderObject(curr) );
//        }
    }
}