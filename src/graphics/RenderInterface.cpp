#include <graphics/RenderInterface.h>

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
