#include <graphics/RenderInterface.h>

std::vector<RenderObject> const &RenderInterface::renderables() const
{
    return m_renderables;
}
