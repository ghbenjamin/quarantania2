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

    [[nodiscard]] Sprite getSprite( std::string const& sheet, std::string const& name );
    [[nodiscard]] Sprite getSprite( SpritesheetKey const& key );
    [[nodiscard]] Sprite getImageAsSprite( std::string const& imgName );
    [[nodiscard]] FontDataPtr getFont(std::string const& fname, int fontSize );
    [[nodiscard]] FontDataPtr getDefaultFont(int fontSize );
    [[nodiscard]] TexturePtr getTexture( std::string const& imgName );
    [[nodiscard]] NinePatchResource const& getNinePatch( std::string const& name );

private:

    ResourceManager() = default;

    void addImageResource( std::string const& name );
    void addFontResource( std::string const& name );
    void addSpritesheetResource( std::string const& name );
    void addNinepatchResource( std::string const& name );

    static const std::string getDefaultFontName();

    std::unordered_map<std::string, std::shared_ptr<FontResource>> m_fonts;
    std::unordered_map<std::string, std::shared_ptr<SpritesheetResource>> m_spritesheets;
    std::unordered_map<std::string, std::shared_ptr<ImageResource>> m_images;
    std::unordered_map<std::string, std::shared_ptr<NinePatchResource>> m_patches;

    WindowPtr m_context;
    FontManager m_fontManager;

};