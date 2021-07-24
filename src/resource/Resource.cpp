#include <resource/Resource.h>

#include <fstream>

#include <resource/ResourceManager.h>
#include <graphics/Shader.h>
#include <utils/Json.h>
#include <utils/Logging.h>

// Resource
// -----------------------

Resource::Resource( std::string const& name )
    : m_isLoaded(false), m_name(name) {}

bool Resource::isLoaded() const
{
    return m_isLoaded;
}


// Image Resource
// -----------------------

ImageResource::ImageResource(std::string const &name)
: Resource(name) {}

const TexturePtr &ImageResource::get() const
{
    return m_texture;
}

void ImageResource::load()
{
    m_texture = std::make_shared<Texture>( "../resource/img/" + m_name + ".png" );
}

void ImageResource::unload()
{
    m_texture.reset();
}

Sprite ImageResource::getSprite() const
{
    return Sprite( m_texture );
}


// Surface Resource
// -----------------------

SurfaceResource::SurfaceResource( std::string const &name )
    : Resource(name) {}

const SurfacePtr &SurfaceResource::get() const
{
    return m_surface;
}

void SurfaceResource::load()
{
    m_surface = std::make_shared<Surface>( "../resource/surfaces/" + m_name + ".png" );
}

void SurfaceResource::unload()
{
    m_surface.reset();
}


// Spritesheet Resource
// -----------------------

SpritesheetResource::SpritesheetResource(std::string const &name)
: Resource(name) {}

const SpritesheetPtr &SpritesheetResource::get() const
{
    return m_spritesheet;
}

void SpritesheetResource::load()
{
    auto tex = std::make_shared<Texture>( "../resource/spritesheet/" + m_name + ".png" );
    auto doc = utils::json::loadFromPath("../resource/spritesheet/" + m_name + ".json" );

    auto metaObj = doc["meta"];
    auto dataObj = doc["data"];

    std::string sheetType = metaObj["type"];
    if ( sheetType == "tiled" )
    {
        std::unordered_map<std::string, int> gidMap;

        for ( auto& node : dataObj.items() )
        {
            gidMap.emplace( node.key(), node.value() );
        }

        int paddingVal = metaObj["shapePadding"];
        int extrudeVal = metaObj["extrude"];
        int marginVal = paddingVal + 2 * extrudeVal;
        
        m_spritesheet = std::make_shared<TiledSpritesheet>( tex, gidMap, marginVal );
    }
    else if ( sheetType == "free" )
    {
        std::unordered_map<std::string, RectI> rectMap;

        for ( auto& node : dataObj.items() )
        {
            auto rectData = node.value();
            rectMap.emplace( node.key(),
                    RectI{ rectData[0],
                           rectData[1],
                           rectData[2],
                           rectData[3]
            });
        }

        m_spritesheet = std::make_shared<FreeSpritesheet>( tex, rectMap );
    }
    else
    {
        AssertAlwaysMsg( fmt::format( "Unknown spriteseet type: {}", sheetType ).c_str() );
    }
}

void SpritesheetResource::unload()
{
    m_spritesheet.reset();
}


// NinePatch Resource
// -----------------------

NinePatchResource::NinePatchResource(const std::string &name)
: Resource(name) {}

void NinePatchResource::load()
{
    m_texture = std::make_shared<Texture>( "../resource/ninepatch/" + m_name + ".png" );
    auto doc = utils::json::loadFromPath("../resource/ninepatch/" + m_name + ".json" );

    m_borderWidth = doc["border_width"];
    auto offsetArr = doc["offsets"];

    for ( int i = 0; i < 9; i++ )
    {
        auto offArr = offsetArr[i];
        RectI offsetRect = {
            offArr[0],
            offArr[1],
            offArr[2],
            offArr[3],
        };

        m_offsets[i] = offsetRect;
    }
}

void NinePatchResource::unload()
{
    m_texture.reset();
    m_borderWidth = -1;
}

int NinePatchResource::borderWidth() const
{
    return m_borderWidth;
}

std::array<RectI, 9> NinePatchResource::offsets() const
{
    return m_offsets;
}

TexturePtr const &NinePatchResource::texture() const
{
    return m_texture;
}

UI::NinePatch NinePatchResource::getPatch() const
{
    return UI::NinePatch { m_texture, m_offsets, m_borderWidth };
}


// Shader Resource
// -----------------------

ShaderResource::ShaderResource( std::string const &name, GLuint type )
 : Resource(name), m_type(type) {}

void ShaderResource::load()
{
    std::string ext;
    
    if (m_type == GL_FRAGMENT_SHADER)
    {
        ext = ".frag";
    }
    else
    {
        ext = ".vert";
    }

    std::ifstream stream( "../resource/shaders/" + m_name + ext );
    std::stringstream buffer;
    buffer << stream.rdbuf();
    
    std::string shaderSource = buffer.str();
    const char* shaderSourceChar = shaderSource.c_str();
    
    m_handle = glCreateShader(m_type);
    glShaderSource(m_handle, 1, &shaderSourceChar, 0);
    glCompileShader(m_handle);
    
    GLint success = 0;
    glGetShaderiv(m_handle, GL_COMPILE_STATUS, &success);
    
    if ( success == GL_FALSE )
    {
        GLint maxLength = 0;
        glGetShaderiv(m_handle, GL_INFO_LOG_LENGTH, &maxLength);
        
        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(m_handle, maxLength, &maxLength, &errorLog[0]);
     
        std::string err( errorLog.begin(), errorLog.end() );
        glDeleteShader(m_handle);
        
        AssertAlwaysMsg( fmt::format( "Error compiling shader {} : {}", m_name, err ) );
    }
}

void ShaderResource::unload()
{
    glDeleteShader(m_handle);
}

GLuint ShaderResource::getType() const
{
    return m_type;
}

GLuint ShaderResource::getHandle() const
{
    return m_handle;
}

ShaderProgramResource::ShaderProgramResource( std::string const &name, std::string const &vertName, std::string const &fragName )
    : Resource(name), m_vertName(vertName), m_fragName(fragName) {}

void ShaderProgramResource::load()
{
    m_program = std::make_shared<ShaderProgram>(m_vertName, m_fragName);
}

void ShaderProgramResource::unload()
{
    m_program.reset();
}

std::shared_ptr<ShaderProgram> ShaderProgramResource::getProgram()
{
    return m_program;
}

GLuint ShaderProgramResource::getHandle() const
{
    return m_program->getHandle();
}

