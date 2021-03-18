#pragma once

#include <string_view>

#pragma warning (disable : 4005)
#include <glad/glad.h>

#include <glm/glm.hpp>


class ShaderProgram
{
public:
    ShaderProgram( std::string const& vertName, std::string const& fragName );
    ~ShaderProgram();

    void useProgram();
    GLuint getHandle();

    GLuint getUniformLocation( std::string const& name );

    void setUniformMat4v( GLuint location, glm::mat4 const& val );
    void setUniformMat4v( std::string const& name, glm::mat4 const& val );


private:

    GLuint m_handle;
};


using ShaderHandle = GLuint;