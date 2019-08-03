#include <graphics/Window.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <utils/Assert.h>

Window::Window(std::string const &title, Vector2i bounds)
: m_size(bounds), m_window(nullptr)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();

    m_window = SDL_CreateWindow(
        title.c_str(),
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        m_size.x(),
        m_size.y(),
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );

    SDL_SetWindowMinimumSize( m_window, 800, 600 );

    Assert( m_window != nullptr);

    m_renderer = std::make_unique<Renderer>(m_window);
}

Window::~Window()
{
    if ( m_window )
    {
        SDL_DestroyWindow( m_window );
    }

    TTF_Quit();
    SDL_Quit();
}

RendererPtr const &Window::renderer()
{
    return m_renderer;
}

SDL_Window *Window::raw()
{
    return m_window;
}

const Vector2i Window::getSize() const
{
    int w = 0;
    int h = 0;

    SDL_GetWindowSize( m_window, &w, &h );
    Assert( w != 0 && h != 0 );

    return Vector2i{w, h};
}
