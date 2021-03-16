#include <graphics/RenderInterface.h>

#include <utils/Logging.h>
#include <engine/Camera.h>

RenderInterface::RenderInterface( Renderer* renderer )
: m_camera(nullptr), m_renderer(renderer) {}

std::vector<RenderObject> const &RenderInterface::renderables() const
{
    return m_renderables;
}

void RenderInterface::clear()
{
    m_renderables.clear();
}

void RenderInterface::setCamera(Camera *camera)
{
    m_camera = camera;
}

void RenderInterface::present()
{
//    std::stable_sort( m_renderables.begin(), m_renderables.end() );
}

void RenderInterface::addItem( RenderObject obj, RenderLayer layer )
{
    if (layer == RenderLayer::UI)
    {
        m_camera->translate(obj);
    }
    
    m_renderer->addItem( obj, layer );
}
