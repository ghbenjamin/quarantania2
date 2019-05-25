#include <utility>

#include <graphics/RenderInterface.h>

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

void RenderInterface::addRenderable(RenderObject const &obj)
{
    m_renderables.push_back(obj);
}


