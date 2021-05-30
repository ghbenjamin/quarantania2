#pragma once

#include <string>
#include <memory>

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

    // Perform initialization that needs to be done after we've loaded our resources e.g. setting
    // up cursors and icons
    void initResources();

    // API

    SDL_Window* raw();
    CursorManager& cursor();
    
    // Update the OpenGL state when the SDL Window changes size
    void onWindowResize( Vector2i screenSize );

    // Current size of our window in pixels
    const Vector2i getSize() const;

    // Swap our front and back buffers
    void swapWindow();


private:
    
    SDL_Window* m_window;
    SDL_GLContext m_glContext;
    Vector2i m_size;
    CursorManager m_cursor;
};


using WindowPtr = std::shared_ptr<Window>;
