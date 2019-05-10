#include <resource/ResourceManager.h>
#include <resource/Window.h>
#include <iostream>
#include <utils/Logging.h>


void ResourceManager::loadAll(std::unique_ptr<Window> const& ctx)
{
    ManifestData data = IO::readResourceManifest();

    loadAllSpritesheets( ctx, data.spriteSheets );
    loadAllImages( ctx, data.images );
    loadAllFonts( ctx, data.fonts );
}


void ResourceManager::loadAllImages(std::unique_ptr<Window> const& ctx,
    std::vector<ManifestImageData> const &data)
{
    for ( auto const& d : data)
    {

    }
}

void ResourceManager::loadAllFonts(std::unique_ptr<Window> const& ctx,
    std::vector<ManifestFontData> const &data)
{
    for ( auto const& d : data)
    {
        m_fonts[d.key] = Font::loadFont( "../resource/font/" + d.path, d.fontSize );
    }
}

void ResourceManager::loadAllSpritesheets(std::unique_ptr<Window> const& ctx,
    std::vector<ManifestSpritesheetData> const &data)
{
    for ( auto const& d : data)
    {
        auto ptr = Texture::loadTexture( ctx->renderer(), "../resource/img/" + d.path );
        m_spritesheets[d.key] = std::make_shared<Spritesheet>( std::move(ptr), d.margin, d.tileSize );
    }
}

