#pragma once

#include <vector>
#include <unordered_map>

#include <graphics/Texture.h>
#include <resource/Spritesheet.h>
#include <resource/Resource.h>
#include <resource/Font.h>
#include <graphics/Text.h>
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
    FontDataPtr getFont(std::string const& fname, int fontSize );
    FontDataPtr getDefaultFont(int fontSize );
    TexturePtr getTexture( std::string const& imgName );
    NinePatchResource const& getNinePatch( std::string const& name );
    GLuint getShader( std::string const& shader );

private:

    ResourceManager() = default;

    void addImageResource( std::string const& name );
    void addFontResource( std::string const& name );
    void addSpritesheetResource( std::string const& name );
    void addNinepatchResource( std::string const& name );
    void addShader( std::string const& name, GLuint type, const char* data );

    static const std::string getDefaultFontName();

    std::unordered_map<std::string, std::shared_ptr<FontResource>> m_fonts;
    std::unordered_map<std::string, std::shared_ptr<SpritesheetResource>> m_spritesheets;
    std::unordered_map<std::string, std::shared_ptr<ImageResource>> m_images;
    std::unordered_map<std::string, std::shared_ptr<NinePatchResource>> m_patches;
    std::unordered_map<std::string, GLuint> m_shaders;

    WindowPtr m_context;
    FontManager m_fontManager;

};