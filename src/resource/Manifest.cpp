#include <resource/Manifest.h>

#include <fstream>
#include <sstream>

#include <rapidjson/document.h>

#include <utils/Assert.h>

ManifestData IO::readResourceManifest( )
{
    ManifestData md;
    std::ifstream fstream( ManifestPath );

    std::stringstream buffer;
    buffer << fstream.rdbuf();

    rapidjson::Document doc;
    doc.Parse( buffer.str().c_str() );

    Assert( doc.IsObject() );
    Assert( !doc.ObjectEmpty() );

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
