#pragma once

#include <graphics/RenderObject.h>

class Renderer;

class RenderInterface
{
public:
    RenderInterface( Renderer* renderer );
    virtual ~RenderInterface() = default;

    // The items which will be rendered to the screen at the end of the frame
    std::vector<RenderObject> const& renderables() const;
    
    void addItem( RenderObject obj, RenderLayer layer );
    
    void setCameraOffset( Vector2f offset );

private:
    Renderer* m_renderer;
    std::vector<RenderObject> m_renderables;
};