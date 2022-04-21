#include <filesystem>

#include <resource/Tiled.h>
#include <utils/Json.h>
#include <utils/Assert.h>
#include <utils/Base64.h>
#include <utils/Compression.h>
#include <utils/Logging.h>
#include <engine/LuaState.h>


TiledMap TiledMapLoader::loadLua( std::string const &path )
{
    LuaState lua;
    
    auto doc = lua.runScriptFile( path );
}


TiledMap TiledMapLoader::loadJson( const std::string &path)
{
    auto doc = utils::json::loadFromPath(path );

    m_map.width = doc["width"];
    m_map.height = doc["height"];
    m_map.tileHeight = doc["tileheight"];
    m_map.tileWidth = doc["tilewidth"];

    for ( auto const& layer : doc["tilesets"] )
    {
        TiledTileset ts;
        ts.filename = layer["source"];
        ts.firstGid = layer["firstgid"];
        ts.sheetName = std::filesystem::path(ts.filename).stem().string();
        m_map.tilesets.push_back(ts);
    }

    for ( auto const& layer : doc["layers"] )
    {
        std::string layerType = layer["type"];
        if ( layerType == "tilelayer" )
        {
            TiledTileLayer ttl;

            ttl.width = layer["width"];
            ttl.height = layer["height"];
            ttl.xOffset = layer["x"];
            ttl.yOffset = layer["y"];
            ttl.name = layer["name"];
            ttl.rawData = layer["data"];
            ttl.encoding = layer["encoding"];
            ttl.compression = layer["compression"];

            m_map.tileLayers.push_back(ttl);
        }
        else if ( layerType == "objectgroup" )
        {
            TiledObjectLayer tol;

            tol.name = layer["name"];
            tol.xOffset = layer["x"];
            tol.yOffset = layer["y"];

            for ( auto const& object : layer["objects"] )
            {
                TiledObjectData tod;

                tod.name = object["name"];

                if ( object.contains("properties") )
                {
                    for ( auto const& prop : object["properties"] )
                    {
                        std::string name = prop["name"];
                        std::string type = prop["type"];
                        JSONValue val;

                        if ( type == "string" )
                        {
                            val = std::string(prop["value"]);
                        }
                        else if ( type == "bool" )
                        {
                            val = prop["value"].get<bool>();
                        }
                        else if ( type == "int" )
                        {
                            val = prop["value"].get<int>();
                        }
                        else if ( type == "color" )
                        {
                            val = std::string(prop["value"]);
                        }
                        else if ( type == "float" )
                        {
                            val = prop["value"].get<float>();
                        }
                        else
                        {
                            AssertAlwaysMsg( "unknown object prop type: " + type );
                        }

                        tod.props.emplace(name, val);
                    }
                }

                if ( object.contains("gid") )
                {
                    int x = object["x"];
                    int y = object["y"];

                    tod.rawPos = { x, y };
                    tod.gid = object["gid"];
                    tod.sprite = resolveGid(tod.gid);
                }
                else if ( object.contains("point") )
                {
                    float x = object["x"];
                    float y = object["y"];

                    tod.rawPos = {
                            (int) std::floorf(x),
                            (int) std::floorf(y)
                    };
                }

                tol.objects.push_back(tod);
            }

            m_map.objectLayers.push_back(tol);
        }
    }

    for (auto& layer : m_map.tileLayers)
    {
        decodeLayer(&layer);
    }

    calculateObjectTilePos();

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
        TiledGid gid = static_cast<TiledGid> (
                  buf[i] |
                  buf[i + 1] << 8 |
                  buf[i + 2] << 16 |
                  buf[i + 3] << 24
            );

        gid &= ~(FLIPPED_HORIZONTALLY_FLAG |
                 FLIPPED_VERTICALLY_FLAG |
                 FLIPPED_DIAGONALLY_FLAG);

        TiledIdPair idp = resolveGid(gid);
        layer->gids.push_back(idp);
    }
}

TiledIdPair TiledMapLoader::resolveGid(TiledGid gid) const
{
    if ( gid != 0 )
    {
        for (int j = (int)m_map.tilesets.size() - 1; j >= 0; j--)
        {
            if ( m_map.tilesets[j].firstGid <= (int) gid )
            {
                return { gid -  m_map.tilesets[j].firstGid, j };
            }
        }
    }

    return {0, 0};
}

void TiledMapLoader::calculateObjectTilePos()
{
    for ( auto& layer : m_map.objectLayers )
    {
        for ( auto& obj : layer.objects )
        {
            int x = layer.xOffset + obj.rawPos.x();
            int y = layer.yOffset + obj.rawPos.y();

            obj.tilePos = { x / m_map.tileWidth, y / m_map.tileHeight };
        }
    }
}



bool TiledIdPair::operator==(const TiledIdPair &rhs) const
{
    return id == rhs.id &&
           tilesheetIdx == rhs.tilesheetIdx;
}

SpritesheetKey TiledMap::keyFromIdPair(const TiledIdPair &idp) const
{
    return SpritesheetKey{ tilesets.at(idp.tilesheetIdx).sheetName, (int) idp.id };
}