#include <graphics/Window.h>

#include <SDL2/SDL_ttf.h>

#pragma warning (disable : 4005)
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <utils/Assert.h>
#include <graphics/RenderInterface.h>
#include <graphics/Shader.h>


Window::Window(std::string const &title, Vector2i bounds)
: m_size(bounds), m_window(nullptr)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
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
    
    m_glContext = SDL_GL_CreateContext(m_window);

    if ( !gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress) )
    {
        std::cerr << "Failed to initialize the OpenGL context." << std::endl;
        exit(1);
    }

//    // Loaded OpenGL successfully.
//    std::cout << "OpenGL version loaded: " << GLVersion.major << "."
//              << GLVersion.minor << std::endl;

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
    
    TTF_Quit();
    SDL_Quit();
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
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    GLuint currTex = 0;

    GLuint modelLoc = m_shaderProgram->getUniformLocation( "model" );

    for ( auto const& item : objs.renderables() )
    {
        glm::vec2 position = {item.screenBounds.x(), item.screenBounds.y()};
        glm::vec2 size = {item.screenBounds.w(), item.screenBounds.h()};
    
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(position, 0.0f));
        model = glm::scale(model, glm::vec3(size, 1.0f));

        m_shaderProgram->setUniformMat4v( modelLoc, model );

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(item.verts), item.verts, GL_STATIC_DRAW);
        
        glBindVertexArray(m_quadVAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

        if ( currTex != item.handle )
        {
            currTex = item.handle;
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, item.handle);
        }
        
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
    
    SDL_GL_SwapWindow(m_window);
}

void Window::openGLSetup()
{
    // No depth testing or face culling
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    
    // Set the viewport size and projection
    glViewport(0, 0, m_size.x(), m_size.y());
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( 0.0, m_size.x(), m_size.y(), 0.0, 1.0, -1.0 );
    
    // Enable alpha blending
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    m_shaderProgram = std::make_shared<ShaderProgram>( "simple_screenspace", "simple_sampler" );
    m_shaderProgram->useProgram();
    GLuint projectionLoc = m_shaderProgram->getUniformLocation( "projection" );

    glm::mat4 projection = glm::ortho(0.0f, (float)m_size.x(), (float)m_size.y(), 0.0f, -1.0f, 1.0f);
    m_shaderProgram->setUniformMat4v( projectionLoc, projection );

    glGenBuffers(1, &m_VBO);
    glGenVertexArrays(1, &m_quadVAO);
}

