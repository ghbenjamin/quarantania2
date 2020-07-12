#include <utility>

#include <graphics/RenderInterface.h>
#include <utils/Logging.h>
#include <engine/Camera.h>

RenderInterface::RenderInterface()
: m_camera(nullptr)
{
}

std::vector<RenderObject> const &RenderInterface::renderables() const
{
    return m_renderables;
}

void RenderInterface::clear()
{
    m_renderables.clear();
}

void RenderInterface::addWorldItem(RenderObject obj)
{
    if ( m_camera->intersects(&obj.targetRect) )
    {
        m_camera->translate(obj.targetRect);
        m_renderables.push_back(obj);
    }
}

void RenderInterface::addScreenItem(RenderObject obj)
{
    m_renderables.push_back(obj);
}

void RenderInterface::setCamera(Camera *camera)
{
    m_camera = camera;
}

void RenderInterface::present()
{
    std::stable_sort( m_renderables.begin(), m_renderables.end(),
            [](auto const& lhs, auto const& rhs) {
        return (int)lhs.renderLayer < (int)rhs.renderLayer;
    });
}
