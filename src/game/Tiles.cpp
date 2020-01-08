#include <game/Tiles.h>
#include <utils/Assert.h>
#include <resource/ResourceManager.h>

Tile const &TileRenderMap::get(TileRef ref) const
{
    return m_tiles.at(ref);
}

TileRef TileRenderMap::addTile(TerrainTile ttype, const SpritesheetKey &key)
{
    Tile t;
    t.sprite = ResourceManager::get().getSprite(key);
    t.sprite.setRenderLayer(RenderLayer::Tiles);

    m_tiles.push_back(t);
    TileRef idx = m_tiles.size() - 1;

    m_names.emplace(ttype, idx);

    return idx;
}

Tile const &TileRenderMap::get(TerrainTile ttype) const
{
    return get(getRef(ttype));
}

TileRef TileRenderMap::getRef(TerrainTile ttype) const
{
    return m_names.at(ttype);
}
