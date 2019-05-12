#pragma once

#include <string>
#include <utils/Containers.h>
#include <graphics/Renderer.h>

class Window
{
public:

    Window( std::string const& title, Vector2i bounds );
    virtual ~Window();

    Window( const Window& ) = delete;
    Window& operator=( const Window& ) = delete;

    SDL_Window* raw();
    RendererPtr const& renderer();

private:

    SDL_Window* m_window;
    Vector2i m_size;

    RendererPtr m_renderer;
};


using WindowPtr = std::shared_ptr<Window>;