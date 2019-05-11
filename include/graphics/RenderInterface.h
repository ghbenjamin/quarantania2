#pragma once

#include <graphics/RenderObject.h>

class RenderInterface
{
public:
    RenderInterface() = default;
    virtual ~RenderInterface() = default;

    std::vector<RenderObject> const& renderables() const;

private:
    std::vector<RenderObject> m_renderables;
};