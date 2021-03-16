#pragma once

#include <graphics/RenderObject.h>


class RenderBuffer
{
public:
    RenderBuffer();
    ~RenderBuffer() = default;
    
    void render();
    void addItem( RenderObject const& robj );

private:
    std::vector<RenderObject> m_data;
    GLuint m_vbo;
    GLuint m_vao;
};


class Renderer
{
public:
    Renderer();
    ~Renderer() = default;
    
    // Delayed initialization until OpenGL context has been created
    void init();
    
    void render();
    void addItem( RenderObject const& robj, RenderLayer layer );
    
private:
    std::vector<RenderBuffer> m_buffers;
    
};