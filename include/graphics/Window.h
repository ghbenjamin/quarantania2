#pragma once

#include <string>

#include <utils/Containers.h>
#include <graphics/CursorManager.h>
#include <graphics/Renderer.h>

namespace GlobalConfig { struct GlobalConfigInfo; }
class RenderInterface;

class Window
{
public:

    Window( GlobalConfig::GlobalConfigInfo const& info );
    virtual ~Window();

    Window( const Window& ) = delete;
    Window& operator=( const Window& ) = delete;

    SDL_Window* raw();
    CursorManager& cursor();
    
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
    CursorManager m_cursor;
    Renderer m_renderer;
};


using WindowPtr = std::shared_ptr<Window>;
