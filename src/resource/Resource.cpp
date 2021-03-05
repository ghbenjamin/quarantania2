#include <resource/Resource.h>
#include <resource/ResourceManager.h>
#include <utils/Json.h>
#include <utils/Logging.h>

Resource::Resource( std::string const& name )
    : m_isLoaded(false), m_name(name) {}

bool Resource::isLoaded() const
{
    return m_isLoaded;
}

ImageResource::ImageResource(std::string const &name)
: Resource(name)
{
}

const TexturePtr &ImageResource::get() const
{
    return m_texture;
}

void ImageResource::load()
{
    m_texture = std::make_shared<Texture>( "../resource/img/" + m_name + ".png" );
}

void ImageResource::unload()
{
    m_texture = TexturePtr();
}

Sprite ImageResource::getSprite() const
{
    return Sprite( m_texture );
}

FontResource::FontResource(std::string const &name)
: Resource(name) {}

FontDataPtr const& FontResource::get(int fontSize)
{
    auto it = m_fonts.find(fontSize);
    if (it == m_fonts.end())
    {
        loadForSize(fontSize);
    }

    return m_fonts.at(fontSize);
}

void FontResource::load()
{
    // Do nothing - we load fonts lazily once a size has been requested
}

void FontResource::unload()
{
    m_fonts.erase( m_fonts.begin(), m_fonts.end() );
}

void FontResource::loadForSize(int fontSize)
{
    m_fonts.emplace(fontSize, FontData::loadFont("../resource/font/" + m_name + ".ttf", fontSize ) );
}

SpritesheetResource::SpritesheetResource(std::string const &name)
: Resource(name) {}

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

    auto tex = std::make_shared<Texture>( "../resource/spritesheet/" + m_name + ".png" );

    rapidjson::Document doc = JsonUtils::loadFromPath( "../resource/spritesheet/" + m_name + ".json" );

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
    m_spritesheet.reset();
}

NinePatchResource::NinePatchResource(const std::string &name)
: Resource(name)
{
}

void NinePatchResource::load()
{
    m_texture = std::make_shared<Texture>( "../resource/ninepatch/" + m_name + ".png" );

    rapidjson::Document doc = JsonUtils::loadFromPath( "../resource/ninepatch/" + m_name + ".json" );

    m_borderWidth = doc.FindMember( "border_width" )->value.GetInt();
    auto offsetArr = doc.FindMember("offsets")->value.GetArray();


    for ( int i = 0; i < 9; i++ )
    {
        auto offArr = offsetArr[i].GetArray();
        RectI offsetRect = {
            offArr[0].GetInt(),
            offArr[1].GetInt(),
            offArr[2].GetInt(),
            offArr[3].GetInt(),
        };

        m_offsets[i] = offsetRect;
    }
}

void NinePatchResource::unload()
{
    m_texture.reset();
    m_borderWidth = -1;
}

int NinePatchResource::borderWidth() const
{
    return m_borderWidth;
}

std::array<RectI, 9> NinePatchResource::offsets() const
{
    return m_offsets;
}

TexturePtr const &NinePatchResource::texture() const
{
    return m_texture;
}

UI::NinePatch NinePatchResource::getPatch() const
{
    return UI::NinePatch { m_texture, m_offsets };
}
