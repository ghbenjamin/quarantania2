#include <resource/ResourceManager.h>
#include <graphics/Window.h>
#include <iostream>
#include <utils/Logging.h>
#include <rapidjson/document.h>
#include <utils/Json.h>


void ResourceManager::registerAll(std::string const &manifest)
{
    ManifestData data = readResourceManifest(manifest);

    for ( auto const& d : data.images )
    {
        addResource<ImageResource>( d.key, d.path );
    }

    for ( auto const& d : data.fonts )
    {
        addResource<FontResource>( d.key, d.path, d.fontSize );
    }

    for ( auto const& d : data.spriteSheets )
    {
        addResource<SpritesheetResource>( d.key, d.path );
    }
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

            mid.key = fontObj.FindMember( "key" )->value.GetString();
            mid.path = fontObj.FindMember( "name" )->value.GetString();
            mid.fontSize = fontObj.FindMember( "fontSize" )->value.GetInt();

            md.fonts.push_back(mid);
        }
    }


    return md;
}

void ResourceManager::setWindow(WindowPtr const &wnd)
{
    m_context = wnd;
}

WindowPtr const &ResourceManager::getWindow() const
{
    return m_context;
}

void ResourceManager::loadAll()
{
    for ( auto const &[k, v] : m_resources )
    {
        v->load();
    }
}

void ResourceManager::unloadAll()
{
    m_resources.clear();
}

Sprite ResourceManager::getSprite(std::string const &sheet, std::string const &name)
{
    try
    {
        return getResource<SpritesheetResource>( sheet )->get()->spriteFromName( name );
    }
    catch ( std::exception const& ex )
    {
        Logging::log( "ERROR: Unknown sprite pair [{}, {}]\n", sheet, name );
        std::terminate();
    }

}

Sprite ResourceManager::getSprite(SpritesheetKey const& key)
{
    return getSprite(key.first, key.second);
}



