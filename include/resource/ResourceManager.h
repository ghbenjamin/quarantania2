#pragma once

#include <vector>
#include <unordered_map>
#include <resource/Manifest.h>
#include <resource/Texture.h>
#include <resource/Spritesheet.h>
#include <resource/Font.h>

class Window;

class ResourceManager
{
public:

    ResourceManager() = default;
    ~ResourceManager() = default;

    ResourceManager( const ResourceManager& ) = delete;
    ResourceManager& operator=( const ResourceManager& ) = delete;

    void loadAll( std::unique_ptr<Window> const& ctx );

private:

    void loadAllSpritesheets( std::unique_ptr<Window> const& ctx, std::vector<ManifestSpritesheetData> const& data );
    void loadAllImages( std::unique_ptr<Window> const& ctx, std::vector<ManifestImageData> const& data );
    void loadAllFonts( std::unique_ptr<Window> const& ctx, std::vector<ManifestFontData> const& data );

    std::unordered_map<std::string, TexturePtr> m_images;
    std::unordered_map<std::string, SpritesheetPtr> m_spritesheets;
    std::unordered_map<std::string, FontPtr> m_fonts;
};