#pragma once

#include <string>
#include <vector>

using TiledGid = std::uint32_t;

struct TiledTileset
{
    std::string filename;
    std::string sheetName;
    int firstGid;
};

struct TiledIdPair
{
    TiledGid id;
    int tilesheetIdx;

    bool operator==(const TiledIdPair &rhs) const;
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
};

struct TiledObjectData
{
    int gid;
    std::string name;
    int rawX;
    int rawY;
    int tileX;
    int tileY;
};

struct TiledObjectLayer
{
    std::string name;
    std::vector<TiledObjectData> objects;

    int xOffset;
    int yOffset;
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
    void calculateObjectTilePos();
    TiledIdPair resolveGid( TiledGid gid ) const;

    TiledMap m_map;
};