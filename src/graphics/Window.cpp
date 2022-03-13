#include <graphics/Window.h>
#include <utils/Assert.h>
#include <graphics/RenderInterface.h>
#include <utils/GlobalConfig.h>

Window::Window(GlobalConfig::GlobalConfigInfo const& info)
: m_window(nullptr)
{
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    if (info.sizeToScreen)
    {
        // If we're max screen size but not fullscreen (default)
        SDL_Rect usableScreen;
        SDL_GetDisplayUsableBounds(0, &usableScreen);

        m_size = { usableScreen.w, usableScreen.h };
    }
    else
    {
        // If we've been forced into windowed mode by our config
        m_size = info.screenSize;
    }
    
    SDL_StartTextInput();

    m_window = SDL_CreateWindow(
        info.windowTitle.c_str(),
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        m_size.x(),
        m_size.y(),
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );

    Assert( m_window != nullptr);

    SDL_SetWindowMinimumSize( m_window, 800, 600 );

    if (info.sizeToScreen)
    {
        SDL_MaximizeWindow( m_window );
        m_size = getSize();
    }

    m_glContext = SDL_GL_CreateContext(m_window);

    if ( !gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress) )
    {
        std::cerr << "Failed to initialize the OpenGL context." << std::endl;
        exit(1);
    }

    SDL_GL_MakeCurrent(m_window, m_glContext);
    SDL_GL_SetSwapInterval(1);
}

Window::~Window()
{
    if (m_glContext)
    {
        SDL_GL_DeleteContext(m_glContext);
    }
    if ( m_window )
    {
        SDL_DestroyWindow( m_window );
    }
    
    SDL_Quit();
}


void Window::initResources()
{
    m_cursor.loadCursors();
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

CursorManager &Window::cursor()
{
    return m_cursor;
}

void Window::swapWindow( )
{
    SDL_GL_SwapWindow(m_window);
}

void Window::onWindowResize( Vector2i screenSize )
{
    m_size = screenSize;
}
