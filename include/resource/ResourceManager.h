#pragma once

#include <vector>
#include <unordered_map>
#include <resource/Manifest.h>
#include <resource/Texture.h>
#include <resource/Spritesheet.h>
#include <resource/Font.h>
#include <graphics/Window.h>

class ResourceManager
{
public:

    explicit ResourceManager( WindowPtr const& ctx );
    ~ResourceManager() = default;

    ResourceManager( const ResourceManager& ) = delete;
    ResourceManager& operator=( const ResourceManager& ) = delete;

    void loadAll( std::string const& manifest );
    void unloadAll();

private:

    ManifestData readResourceManifest( std::string const& path);

    void loadAllSpritesheets( std::vector<ManifestSpritesheetData> const& data );
    void loadAllImages( std::vector<ManifestImageData> const& data );
    void loadAllFonts( std::vector<ManifestFontData> const& data );

    WindowPtr m_context;

    std::unordered_map<std::string, TexturePtr> m_images;
    std::unordered_map<std::string, SpritesheetPtr> m_spritesheets;
    std::unordered_map<std::string, FontPtr> m_fonts;
};