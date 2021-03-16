#include <graphics/RenderInterface.h>

#include <graphics/Renderer.h>
#include <utils/Logging.h>

RenderInterface::RenderInterface( Renderer* renderer )
: m_renderer(renderer) {}

std::vector<RenderObject> const &RenderInterface::renderables() const
{
    return m_renderables;
}

void RenderInterface::addItem( RenderObject obj, RenderLayer layer )
{
    m_renderer->addItem( obj, layer );
}

void RenderInterface::setCameraOffset( Vector2f offset )
{
    m_renderer->setCameraOffset( offset );
}
