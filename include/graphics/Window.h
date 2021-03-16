#pragma once

#include <string>

#include <utils/Containers.h>
#include <graphics/Cursor.h>
#include <graphics/Renderer.h>

class RenderInterface;

class Window
{
public:

    Window( std::string const& title, Vector2i bounds );
    virtual ~Window();

    Window( const Window& ) = delete;
    Window& operator=( const Window& ) = delete;

    SDL_Window* raw();
    Cursor& cursor();
    
    // Update the OpenGL state when the SDL Window changes size
    void onWindowResize( Vector2i screenSize );
    
    const Vector2i getSize() const;
    void render( RenderInterface const& objs );
    void openGLSetup();
    
    RenderInterface createRenderInterface();

private:
    
    SDL_Window* m_window;
    SDL_GLContext m_glContext;
    Vector2i m_size;
    Cursor m_cursor;
    
    Renderer m_renderer;
    std::shared_ptr<ShaderProgram> m_shaderProgram;
};


using WindowPtr = std::shared_ptr<Window>;
