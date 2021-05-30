#include <graphics/RenderInterface.h>

#include <graphics/Renderer.h>
#include <utils/Logging.h>

RenderInterface::RenderInterface( Renderer* renderer )
: m_renderer(renderer) {}

void RenderInterface::addItem( RenderObject obj, RenderLayer layer )
{
    if (m_currentScissor.has_value())
    {
        obj.setScissor(*m_currentScissor);
    }
    
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

void RenderInterface::setScissor( RectI scissor )
{
    m_currentScissor = scissor;
}

void RenderInterface::removeScissor()
{
    m_currentScissor.reset();
}

void RenderInterface::startFadeout( float time )
{
    m_renderer->startFadeout(time);
}

void RenderInterface::startFadein( float time )
{
    m_renderer->startFadein( time );
}
