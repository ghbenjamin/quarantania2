#include <resource/ResourceManager.h>
#include <graphics/Window.h>
#include <iostream>
#include <utils/Logging.h>
#include <rapidjson/document.h>
#include <utils/Json.h>

ResourceManager::ResourceManager(WindowPtr const &ctx)
: m_context(ctx)
{
}

void ResourceManager::loadAll( std::string const& manifest )
{
    ManifestData data = readResourceManifest(manifest);

    loadAllSpritesheets( data.spriteSheets );
    loadAllImages( data.images );
    loadAllFonts( data.fonts );
}

void ResourceManager::unloadAll()
{
    m_fonts.clear();
    m_images.clear();
    m_spritesheets.clear();
}

ManifestData ResourceManager::readResourceManifest( std::string const& path )
{
    ManifestData md;
    rapidjson::Document doc = JsonUtils::loadFromPath( path );

    {
        auto it = doc.FindMember( "spritesheet" );
        auto imgArr = it->value.GetArray();

        for ( auto const& imgNode : imgArr )
        {
            auto imgObj = imgNode.GetObject();
            ManifestSpritesheetData mid;

            mid.key = imgObj.FindMember( "key" )->value.GetString();
            mid.path = imgObj.FindMember( "name" )->value.GetString();
            mid.margin = imgObj.FindMember( "margin" )->value.GetInt();
            mid.tileSize = imgObj.FindMember( "tileSize" )->value.GetInt();

            md.spriteSheets.push_back(mid);
        }
    }

    {
        auto it = doc.FindMember( "img" );
        auto imgArr = it->value.GetArray();

        for ( auto const& imgNode : imgArr )
        {
            auto imgObj = imgNode.GetObject();
            ManifestImageData mid;

            mid.key = imgObj.FindMember( "key" )->value.GetString();
            mid.path = imgObj.FindMember( "name" )->value.GetString();

            md.images.push_back(mid);
        }
    }

    {
        auto it = doc.FindMember( "font" );
        auto fontArr = it->value.GetArray();

        for ( auto const& fontNode : fontArr )
        {
            auto fontObj = fontNode.GetObject();
            ManifestFontData mid;

            mid.path = fontObj.FindMember( "name" )->value.GetString();

            md.fonts.push_back(mid);
        }
    }


    return md;
}



void ResourceManager::loadAllImages(std::vector<ManifestImageData> const &data)
{
    for ( auto const& d : data)
    {

    }
}

void ResourceManager::loadAllFonts(std::vector<ManifestFontData> const &data)
{
    for ( auto const& d : data)
    {
        m_fonts[d.key] = Font::loadFont( "../resource/font/" + d.path, d.fontSize );
    }
}

void ResourceManager::loadAllSpritesheets(std::vector<ManifestSpritesheetData> const &data)
{
    for ( auto const& d : data)
    {
        auto ptr = Texture::loadTexture( m_context->renderer()->raw(), "../resource/img/" + d.path );
        m_spritesheets[d.key] = std::make_shared<Spritesheet>( std::move(ptr), d.margin, d.tileSize );
    }
}


