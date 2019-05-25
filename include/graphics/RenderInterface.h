#pragma once

#include <graphics/RenderObject.h>
#include <graphics/Window.h>

class RenderInterface
{
public:
    explicit RenderInterface( WindowCPtr ) ;
    virtual ~RenderInterface() = default;

    std::vector<RenderObject> const& renderables() const;
    void clear();

    void addRenderable( RenderObject const& obj );



private:

    std::vector<RenderObject> m_renderables;
    WindowCPtr m_window;
};