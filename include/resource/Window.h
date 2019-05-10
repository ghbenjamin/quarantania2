#pragma once

#include <string>
#include <utils/Containers.h>

class Window
{
public:

    Window( std::string const& title, Vector2i bounds );
    virtual ~Window();

    Window( const Window& ) = delete;
    Window& operator=( const Window& ) = delete;

    SDL_Renderer* renderer() const;

private:

    SDL_Window* m_window;
    SDL_Renderer* m_renderer;

    Vector2i m_size;

};