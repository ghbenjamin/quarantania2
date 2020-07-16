#pragma once

#include <string>
#include <vector>

struct TiledTileset
{
    std::string name;
    int firstGid;
};

struct TiledIdPair
{
    int id;
    int tilesheedIdx;
};

struct TiledTileLayer
{
    std::string name;
    std::string rawData;
    std::string compression;
    std::string encoding;

    std::vector<TiledIdPair> gids;

    int width;
    int height;
    int xOffset;
    int yOffset;


    void decode();
};

struct TiledObjectLayer
{

};

struct TiledMap
{
    int width;
    int height;
    int tileWidth;
    int tileHeight;

    std::vector<TiledTileLayer> tileLayers;
    std::vector<TiledObjectLayer> objectLayers;
    std::vector<TiledTileset> tilesets;
};

class TiledMapLoader
{
public:
    TiledMapLoader() = default;
    ~TiledMapLoader() = default;

    TiledMap load( std::string const& path );

private:
    void decodeLayer(TiledTileLayer* layer);

    TiledMap m_map;
};