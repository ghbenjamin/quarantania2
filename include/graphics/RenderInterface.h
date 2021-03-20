#pragma once

#include <graphics/RenderObject.h>

class Renderer;

class RenderInterface
{
public:
    RenderInterface( Renderer* renderer );
    ~RenderInterface() = default;
    
    void addItem( RenderObject obj, RenderLayer layer );
    void setCameraOffset( Vector2f offset );

private:
    Renderer* m_renderer;
};