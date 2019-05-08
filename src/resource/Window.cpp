#include <resource/Window.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <utils/Assert.h>

Window::Window(std::string const &title, Vector2i bounds)
: m_size(bounds), m_window(nullptr), m_renderer(nullptr)
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
        SDL_WINDOW_OPENGL
    );

    Assert( m_window != nullptr);

    m_renderer = SDL_CreateRenderer(
        m_window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    Assert( m_renderer != nullptr );
}

Window::~Window()
{
    if ( m_renderer )
    {
        SDL_DestroyRenderer( m_renderer );
    }

    if ( m_window )
    {
        SDL_DestroyWindow( m_window );
    }

    TTF_Quit();
    SDL_Quit();
}
