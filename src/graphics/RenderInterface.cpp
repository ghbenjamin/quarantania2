#include <utility>

#include <graphics/RenderInterface.h>
#include <utils/Logging.h>

RenderInterface::RenderInterface(WindowCPtr window)
: m_window(std::move(window))
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
    if ( m_camera.intersects(&obj.targetRect) )
    {
        auto [x, y] = m_camera.getPosition();
        obj.targetRect.x -= x;
        obj.targetRect.y -= y;

        m_renderables.push_back(obj);
    }
}

void RenderInterface::addScreenItem(RenderObject obj)
{
    m_renderables.push_back(obj);
}

const WindowCPtr &RenderInterface::window() const
{
    return m_window;
}

Camera &RenderInterface::camera()
{
    return m_camera;
}


