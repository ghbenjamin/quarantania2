#include <filesystem>

#include <resource/Tiled.h>
#include <utils/Assert.h>
#include <utils/Base64.h>
#include <utils/Compression.h>
#include <engine/LuaState.h>


TiledMap TiledMapLoader::loadLua( std::string const &path )
{
    LuaState lua;
    sol::table doc = lua.runLoadedScript( path );

    m_map.width = doc["width"];
    m_map.height = doc["height"];
    m_map.tileHeight = doc["tileheight"];
    m_map.tileWidth = doc["tilewidth"];

    for ( auto const& [k, v] : doc["tilesets"].get<sol::table>() )
    {
        sol::table const& obj = v;
        TiledTileset ts;
        ts.filename = obj["filename"].get<sol::string_view>();
        ts.firstGid = obj["firstgid"];
        ts.sheetName = std::filesystem::path(ts.filename).stem().string();
        m_map.tilesets.push_back(ts);
    }

    for ( auto const& [k, v] : doc["layers"].get<sol::table>() )
    {
        sol::table const& layer = v;
        std::string layerType = layer["type"];
        if ( layerType == "tilelayer" )
        {
            TiledTileLayer ttl;

            ttl.width = layer["width"];
            ttl.height = layer["height"];
            ttl.xOffset = layer["x"];
            ttl.yOffset = layer["y"];
            ttl.name = layer["name"].get<sol::string_view>();
            ttl.rawData = layer["data"].get<sol::string_view>();
            ttl.encoding = layer["encoding"].get<sol::string_view>();
            ttl.compression = layer["compression"].get<sol::string_view>();

            m_map.tileLayers.push_back(ttl);
        }
        else if ( layerType == "objectgroup" )
        {
            TiledObjectLayer tol;

            tol.name = layer["name"].get<sol::string_view>();;
            tol.xOffset = int(layer["offsetx"].get<float>());
            tol.yOffset = int(layer["offsety"].get<float>());

            for ( auto const& [k2, v2] : layer["objects"].get<sol::table>() )
            {
                TiledObjectData tod;
                sol::table const& object = v2;

                tod.name = object["name"].get<sol::string_view>();

                auto propsVal = object["properties"];
                if ( propsVal != sol::nil )
                {
                    for ( auto const& [k3, v3] : propsVal.get<sol::table>() )
                    {
                        std::string casted;
                        
                        if ( v3.get_type() == sol::type::number )
                        {
                            casted = std::to_string( v3.as<float>() );
                        }
                        else if ( v3.get_type() == sol::type::boolean )
                        {
                            casted = std::to_string( v3.as<bool>() );
                        }
                        else
                        {
                            casted = v3.as<sol::string_view>();
                        }
                        
                        tod.props.emplace( k3.as<sol::string_view>(), casted );
                    }
                }


                if ( object["gid"] != sol::nil )
                {
                    tod.gid = object["gid"];
                    tod.sprite = resolveGid(tod.gid);
                }
                

                auto shapeVal = object["shape"];
                if ( shapeVal != sol::nil )
                {
                    auto const& shapeStr = shapeVal.get<std::string_view>();
                    
                    if ( shapeStr == "rectangle" )
                    {
                        int x = object["x"];
                        int y = object["y"];
    
                        tod.rawPos = { x, y };
                    }
                    else if ( shapeStr == "point" )
                    {
                        float x = object["x"];
                        float y = object["y"];
    
                        tod.rawPos = {
                                (int) std::floorf(x),
                                (int) std::floorf(y)
                        };
                    }
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