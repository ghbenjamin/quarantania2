#pragma once

#include <string>

#include <utils/Containers.h>
#include <graphics/Renderer.h>
#include <graphics/Cursor.h>

class Window
{
public:

    Window( std::string const& title, Vector2i bounds );
    virtual ~Window();

    Window( const Window& ) = delete;
    Window& operator=( const Window& ) = delete;

    SDL_Window* raw();
    RendererPtr const& renderer();
    Cursor& cursor();

    const Vector2i getSize() const;

private:

    SDL_Window* m_window;
    Vector2i m_size;

    RendererPtr m_renderer;
    Cursor m_cursor;
};


using WindowPtr = std::shared_ptr<Window>;
using WindowCPtr = std::shared_ptr<const Window>;