#include <resource/Resource.h>
#include <resource/ResourceManager.h>
#include <utils/Json.h>
#include <utils/Logging.h>

Resource::Resource( std::string const& key, std::string const& path )
    : m_isLoaded(false), m_type(ResourceType::Null), m_key(key), m_path(path)
{

}

std::string const &Resource::getPath() const
{
    return m_path;
}

bool Resource::isLoaded() const
{
    return m_isLoaded;
}

ResourceType Resource::getType() const
{
    return m_type;
}

std::string const &Resource::getKey() const
{
    return m_key;
}

ImageResource::ImageResource(std::string const &key, std::string const &path)
: Resource(key, path)
{
    m_type = ResourceType::Image;
}

const TexturePtr &ImageResource::get() const
{
    return m_texture;
}

void ImageResource::load()
{
    m_texture = Texture::loadTexture( "../resource/img/" + m_path + ".png" );
}

void ImageResource::unload()
{
    m_texture = TexturePtr();
}

Sprite ImageResource::getSprite() const
{
    return Sprite( m_texture );
}

FontResource::FontResource(std::string const &key, std::string const &path, int fontSize)
: Resource(key, path), m_fontSize(fontSize)
{
    m_type = ResourceType::Font;
}

const FontPtr &FontResource::get() const
{
    return m_font;
}

void FontResource::load()
{
    m_font = Font::loadFont( "../resource/font/" + m_path, m_fontSize );
}

void FontResource::unload()
{
    m_font = FontPtr();
}

SpritesheetResource::SpritesheetResource(std::string const &key, std::string const &path)
: Resource(key, path)
{
    m_type = ResourceType::Spritesheet;
}

const SpritesheetPtr &SpritesheetResource::get() const
{
    return m_spritesheet;
}

void SpritesheetResource::load()
{
// MSVC compiler error
#ifdef GetObject
#undef GetObject
#endif

    auto tex = Texture::loadTexture( "../resource/spritesheet/" + m_path + ".png" );

    rapidjson::Document doc = JsonUtils::loadFromPath( "../resource/spritesheet/" + m_path + ".json" );

    auto metaObj = doc.FindMember( "meta" )->value.GetObject();
    auto dataObj = doc.FindMember( "data" )->value.GetObject();

    std::string sheetType = metaObj.FindMember("type")->value.GetString();
    if ( sheetType == "tiled" )
    {
        std::unordered_map<std::string, int> gidMap;

        for ( auto& node : dataObj )
        {
            gidMap.emplace( node.name.GetString(), node.value.GetInt() );
        }

        int marginVal = metaObj.FindMember( "margin" )->value.GetInt();
        m_spritesheet = std::make_shared<TiledSpritesheet>( tex, gidMap, marginVal );
    }
    else if ( sheetType == "free" )
    {
        std::unordered_map<std::string, RectI> rectMap;

        for ( auto& node : dataObj )
        {
            auto rectData = node.value.GetArray();
            rectMap.emplace( node.name.GetString(),
                    RectI{ rectData[0].GetInt(),
                           rectData[1].GetInt(),
                           rectData[2].GetInt(),
                           rectData[3].GetInt()
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
    m_spritesheet = SpritesheetPtr();
}
