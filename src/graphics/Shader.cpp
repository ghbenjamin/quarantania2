#include <graphics/Shader.h>
#include <resource/ResourceManager.h>
#include <fmt/format.h>
#include <utils/Logging.h>

#include <glm/gtc/type_ptr.hpp>



ShaderProgram::ShaderProgram(std::string const& vertName, std::string const& fragName)
{
    auto& shVert = ResourceManager::get().getShader( vertName );
    auto& shFrag = ResourceManager::get().getShader( fragName );

    m_handle = glCreateProgram();
    glAttachShader( m_handle, shVert.getHandle() );
    glAttachShader( m_handle, shFrag.getHandle() );

    glLinkProgram( m_handle );

    GLint success = 0;
    glGetProgramiv( m_handle, GL_LINK_STATUS, &success );
    if (success == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetProgramiv(m_handle, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(m_handle, maxLength, &maxLength, &infoLog[0]);

        std::string err( infoLog.begin(), infoLog.end() );

        glDeleteProgram(m_handle);

        Logging::log( fmt::format( "Error linking shader from {} and {}:\n{}", vertName, fragName, err));
    }
}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram( m_handle );
}

void ShaderProgram::useProgram()
{
    glUseProgram( m_handle );
}

GLuint ShaderProgram::getUniformLocation(const std::string& name)
{
    return glGetUniformLocation(m_handle, name.c_str());
}

void ShaderProgram::setUniformMat4v(GLuint location, const glm::mat4 &val)
{
    glUniformMatrix4fv( location, 1, false, glm::value_ptr(val) );
}

GLuint ShaderProgram::getHandle()
{
    return m_handle;
}
