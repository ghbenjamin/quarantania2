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
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
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


void Window::openGLSetup()
{
    // No depth testing or face culling
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    
    // Set the viewport size and projection
    glViewport(0, 0, m_size.x(), m_size.y());
    
    // Enable alpha blending
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    m_shaderProgram = std::make_shared<ShaderProgram>( "simple_screenspace", "simple_sampler" );
    m_shaderProgram->useProgram();
    
    GLuint projectionLoc = m_shaderProgram->getUniformLocation( "projection" );
    glm::mat4 projection = glm::ortho(0.0f, (float)m_size.x(), (float)m_size.y(), 0.0f, -1.0f, 1.0f);
    m_shaderProgram->setUniformMat4v( projectionLoc, projection );
    
    GLuint modelLoc = m_shaderProgram->getUniformLocation( "model" );
    glm::mat4 identity = glm::mat4(1.0f);
    m_shaderProgram->setUniformMat4v( modelLoc, identity );
    
    glGenBuffers(1, &m_VBO);
    glGenVertexArrays(1, &m_quadVAO);
    
    
    m_renderer.init();
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
//    GLuint currTex = 0;
    
    
    m_renderer.render();
    
    
//
//    for ( auto const& item : objs.renderables() )
//    {
//        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
//        glBufferData(GL_ARRAY_BUFFER, item.getVerts().size() * sizeof(GLfloat) * 48, item.getVerts().data(), GL_STATIC_DRAW);
//
//        glBindVertexArray(m_quadVAO);
//        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
//        glEnableVertexAttribArray(0);
//        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(4 * sizeof(float)));
//        glEnableVertexAttribArray(1);
//
//
//        if ( currTex != item.getHandle() )
//        {
//            currTex = item.getHandle();
//            glActiveTexture(GL_TEXTURE0);
//            glBindTexture(GL_TEXTURE_2D, item.getHandle());
//        }
//
//        glDrawArrays(GL_TRIANGLES, 0, 6);
//    }
    
    SDL_GL_SwapWindow(m_window);
}

void Window::onWindowResize( Vector2i screenSize )
{
    glViewport(0, 0, screenSize.x(), screenSize.y());
}

RenderInterface Window::getRenderInterface()
{
    return RenderInterface( &m_renderer );
}

