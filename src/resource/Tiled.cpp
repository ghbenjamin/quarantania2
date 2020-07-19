#include <filesystem>

#include <resource/Tiled.h>
#include <utils/Json.h>
#include <utils/Assert.h>
#include <utils/Base64.h>
#include <utils/Compression.h>
#include <utils/Logging.h>

TiledMap TiledMapLoader::load(const std::string &path)
{
    rapidjson::Document doc = JsonUtils::loadFromPath( path );

    m_map.width = doc.FindMember("width")->value.GetInt();
    m_map.height = doc.FindMember("height")->value.GetInt();
    m_map.tileHeight = doc.FindMember("tileheight")->value.GetInt();
    m_map.tileWidth = doc.FindMember("tilewidth")->value.GetInt();

    for ( auto const& layer : doc.FindMember("tilesets")->value.GetArray() )
    {
        TiledTileset ts;
        ts.filename = layer.FindMember("source")->value.GetString();
        ts.firstGid = layer.FindMember("firstgid")->value.GetInt();
        ts.sheetName = std::filesystem::path(ts.filename).stem().string();
        m_map.tilesets.push_back(ts);
    }

    for ( auto const& layer : doc.FindMember("layers")->value.GetArray() )
    {
        std::string layerType = layer.FindMember("type")->value.GetString();
        if ( layerType == "tilelayer" )
        {
            TiledTileLayer ttl;

            ttl.width = layer.FindMember("width")->value.GetInt();
            ttl.height = layer.FindMember("height")->value.GetInt();
            ttl.xOffset = layer.FindMember("x")->value.GetInt();
            ttl.yOffset = layer.FindMember("y")->value.GetInt();
            ttl.name = layer.FindMember("name")->value.GetString();
            ttl.rawData = layer.FindMember("data")->value.GetString();
            ttl.encoding = layer.FindMember("encoding")->value.GetString();
            ttl.compression = layer.FindMember("compression")->value.GetString();

            m_map.tileLayers.push_back(ttl);
        }
        else if ( layerType == "objectgroup" )
        {
            TiledObjectLayer tol;

            m_map.objectLayers.push_back(tol);
        }
    }

    for (auto& layer : m_map.tileLayers)
    {
        decodeLayer(&layer);
    }

    return m_map;
}

void TiledMapLoader::decodeLayer(TiledTileLayer *layer)
{
    static constexpr unsigned FLIPPED_HORIZONTALLY_FLAG = 0x80000000;
    static constexpr unsigned FLIPPED_VERTICALLY_FLAG   = 0x40000000;
    static constexpr unsigned FLIPPED_DIAGONALLY_FLAG   = 0x20000000;

    std::string strbuf;
    std::vector<unsigned char> buf;

    if ( layer->encoding == "base64")
    {
        strbuf = base64_decode(layer->rawData);
    }

    if (layer->compression == "zlib")
    {
        strbuf = zlib_decompress(strbuf);
    }

    Assert( strbuf.size() == layer->width * layer->height * 4 );

    buf = std::vector<unsigned char>( strbuf.begin(), strbuf.end() );

    for (int i = 0; i < (int) buf.size(); i += 4)
    {
        TiledIdPair idp;

        std::uint32_t gid = static_cast<std::uint32_t> (
                  buf[i] |
                  buf[i + 1] << 8 |
                  buf[i + 2] << 16 |
                  buf[i + 3] << 24
            );

        gid &= ~(FLIPPED_HORIZONTALLY_FLAG |
                 FLIPPED_VERTICALLY_FLAG |
                 FLIPPED_DIAGONALLY_FLAG);

        if ( gid == 0 )
        {
            idp = {0, 0};
        }
        else
        {
            for (int j = (int)m_map.tilesets.size() - 1; j >= 0; j--)
            {
                if ( m_map.tilesets[j].firstGid <= gid )
                {
                    idp = { gid - m_map.tilesets[j].firstGid, j };
                    break;
                }
            }
        }

        layer->gids.push_back(idp);
    }
}

bool TiledIdPair::operator==(const TiledIdPair &rhs) const
{
    return id == rhs.id &&
           tilesheetIdx == rhs.tilesheetIdx;
}
