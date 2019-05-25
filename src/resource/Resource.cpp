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
    m_texture = Texture::loadTexture( ResourceManager::get().getWindow()->renderer()->raw(),
                                      "../resource/img/" + m_path );
}

void ImageResource::unload()
{
    m_texture = TexturePtr();
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

SpritesheetResource::SpritesheetResource(std::string const &key, std::string const &path, int margin, int tileSize)
: Resource(key, path), m_margin(margin), m_tileSize(tileSize)
{
    m_type = ResourceType::Spritesheet;
}

const SpritesheetPtr &SpritesheetResource::get() const
{
    return m_spritesheet;
}

void SpritesheetResource::load()
{
#undef GetObject

    auto tex = Texture::loadTexture(
        ResourceManager::get().getWindow()->renderer()->raw(),
        "../resource/spritesheet/" + m_path + ".png"
    );

    std::unordered_map<std::string, int> gidMap;
    rapidjson::Document doc = JsonUtils::loadFromPath( "../resource/spritesheet/" + m_path + ".json" );

    for ( auto& node : doc.GetObject() )
    {
       gidMap.emplace( node.name.GetString(), node.value.GetInt() );
    }

    m_spritesheet = std::make_shared<Spritesheet>( tex, gidMap, m_margin, m_tileSize );
}

void SpritesheetResource::unload()
{
    m_spritesheet = SpritesheetPtr();
}
