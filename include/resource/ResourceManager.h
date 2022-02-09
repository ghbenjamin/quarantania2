#pragma once

#include <vector>
#include <unordered_map>

#include <graphics/Texture.h>
#include <resource/Spritesheet.h>
#include <resource/Resource.h>
#include <graphics/Window.h>
#include <utils/Assert.h>

class ResourceManager
{
public:

    static ResourceManager& get()
    {
        static ResourceManager instance;
        return instance;
    }

    ResourceManager( const ResourceManager& ) = delete;
    ResourceManager& operator=( const ResourceManager& ) = delete;
    ~ResourceManager() = default;

    void setWindow( WindowPtr const& wnd );
    WindowPtr const& getWindow() const;

    void loadAll();
    void unloadAll();

    Sprite getSprite( std::string const& sheet, std::string const& name );
    Sprite getSprite( SpritesheetKey const& key );
    Sprite getImageAsSprite( std::string const& imgName );
    SpritesheetResource const& getSpritesheet( std::string const& sheetName );
    std::shared_ptr<FtFontFace> getFont( std::string const& fname );
    std::shared_ptr<FtFontFace> getDefaultFont();
    TexturePtr getTexture( std::string const& imgName );
    SurfacePtr getSurface( std::string const& surfaceName );
    NinePatchResource const& getNinePatch( std::string const& name );
    ShaderResource const& getShader( std::string const& name );
    ShaderProgramResource& getShaderProgram ( std::string const& name );
    TextResource& getXMLDoc ( std::string const& name );

private:

    ResourceManager() = default;

    void addImageResource( std::string const& name );
    void addSurfaceResource( std::string const& name );
    void addSpritesheetResource( std::string const& name );
    void addNinepatchResource( std::string const& name );
    void addShaderResource( std::string const& name, GLuint type );
    void addShaderProgram( std::string const& name, std::string const& vertName, std::string const& fragName );
    void addXMLDoc( std::string const& name, std::string const& path );
    
    static const std::string getDefaultFontName();

    std::unordered_map<std::string, std::shared_ptr<SpritesheetResource>> m_spritesheets;
    std::unordered_map<std::string, std::shared_ptr<ImageResource>> m_images;
    std::unordered_map<std::string, std::shared_ptr<SurfaceResource>> m_surfaces;
    std::unordered_map<std::string, std::shared_ptr<NinePatchResource>> m_patches;
    std::unordered_map<std::string, std::shared_ptr<ShaderResource>> m_shaders;
    std::unordered_map<std::string, std::shared_ptr<ShaderProgramResource>> m_shaderProgs;
    std::unordered_map<std::string, std::shared_ptr<TextResource>> m_xml;

    FtFontManager m_fontManager;
    WindowPtr m_context;
};