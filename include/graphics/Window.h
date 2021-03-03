#pragma once

#include <string>

#include <utils/Containers.h>
#include <graphics/Cursor.h>

class RenderInterface;


class Window
{
public:

    Window( std::string const& title, Vector2i bounds );
    virtual ~Window();

    Window( const Window& ) = delete;
    Window& operator=( const Window& ) = delete;

    SDL_Window* raw();
    SDL_Renderer* renderer();
    Cursor& cursor();

    const Vector2i getSize() const;
    
    void render( RenderInterface const& objs );

private:

    SDL_Window* m_window;
    SDL_Renderer* m_renderer;

#ifdef USE_GL
    SDL_GLContext m_glContext;
#endif

    Vector2i m_size;
    Cursor m_cursor;
};


using WindowPtr = std::shared_ptr<Window>;
