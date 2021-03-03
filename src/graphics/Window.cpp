#include <graphics/Window.h>

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <glad/glad.h>

#include <utils/Assert.h>
#include <graphics/RenderInterface.h>


Window::Window(std::string const &title, Vector2i bounds)
: m_size(bounds), m_window(nullptr)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
    
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    m_window = SDL_CreateWindow(
        title.c_str(),
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        m_size.x(),
        m_size.y(),
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );
    Assert( m_window != nullptr);

    SDL_SetWindowMinimumSize( m_window, 800, 600 );
    
    m_renderer = SDL_CreateRenderer( m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
    Assert( m_renderer != nullptr );
    
    
    // DEBUG

//    m_glContext = SDL_GL_CreateContext(m_window);
//
//    if (!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress)) {
//        std::cerr << "Failed to initialize the OpenGL context." << std::endl;
//        exit(1);
//    }
//
//    // Loaded OpenGL successfully.
//    std::cout << "OpenGL version loaded: " << GLVersion.major << "."
//              << GLVersion.minor << std::endl;
//
//    glDisable(GL_DEPTH_TEST);
//    glDisable(GL_CULL_FACE);
//    glViewport(0, 0, m_size.x(), m_size.y());
//    glMatrixMode( GL_PROJECTION );
//    glLoadIdentity();
//    glOrtho( 0.0, m_size.x(), m_size.y(), 0.0, 1.0, -1.0 );
//
//    SDL_GL_MakeCurrent(m_window, m_glContext);
//    SDL_GL_SetSwapInterval(1);
}

Window::~Window()
{
//    if (m_glContext)
//    {
//        SDL_GL_DeleteContext(m_glContext);
//    }
    
    if ( m_window )
    {
        SDL_DestroyWindow( m_window );
    }
    
    TTF_Quit();
    
    SDL_Quit();
    
    if ( m_renderer )
    {
        SDL_DestroyRenderer(m_renderer);
    }
}

SDL_Renderer* Window::renderer()
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

Cursor &Window::cursor()
{
    return m_cursor;
}

void Window::render( RenderInterface const &objs )
{
    SDL_RenderClear( m_renderer );
    SDL_SetRenderDrawBlendMode( m_renderer, SDL_BLENDMODE_BLEND );
    SDL_Rect const* clipRect = NULL;
    
    for ( auto const& item : objs.renderables() )
    {
        if ( !SDL_RectEmpty( &item.clipRect ) )
        {
            if ( !SDL_RectEquals( clipRect, &item.clipRect ) )
            {
                // If this item has a clip rect, and it's different to our current clip rect, set the cliprect
                SDL_RenderSetClipRect( m_renderer, &item.clipRect );
                clipRect = &item.clipRect;
            }
        }
        else if ( clipRect != NULL )
        {
            // Otherwise, if the requested clip is null, and the current clip is not, remove the clip
            clipRect = NULL;
            SDL_RenderSetClipRect( m_renderer, NULL );
        }
        
        // If the current clip is the same as the requested clip, do nothing
        
        // Render the item, respecting the above clip
        SDL_RenderCopy(m_renderer, item.texture, &item.sourceRect, &item.targetRect);
    }
    
    SDL_RenderPresent( m_renderer );
}
