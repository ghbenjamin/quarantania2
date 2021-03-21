#include <graphics/RenderInterface.h>

#include <graphics/Renderer.h>
#include <utils/Logging.h>

RenderInterface::RenderInterface( Renderer* renderer )
: m_renderer(renderer) {}

void RenderInterface::addItem( RenderObject obj, RenderLayer layer )
{
    m_renderer->addItem( obj, layer );
}

void RenderInterface::setCameraOffset( Vector2f offset )
{
    m_renderer->setCameraOffset( offset );
}

void RenderInterface::holdRenderQueue(RenderLayer layer)
{
    m_renderer->holdBuffer(layer);
}

void RenderInterface::releaseRenderQueue(RenderLayer layer)
{
    m_renderer->releaseBuffer(layer);
}
