#include <game/Tiles.h>
#include <utils/Assert.h>
#include <resource/ResourceManager.h>

TileRef TileRenderMap::addTile(const SpritesheetKey& key, bool passible)
{
    auto const&[sheetName, tileName] = key;
    auto sheetPtr = ResourceManager::get().getResource<SpritesheetResource>(sheetName)->get();

    Tile t;
    t.sprite = sheetPtr->spriteFromName( tileName );
    t.passible = passible;

    m_tiles.push_back(t);
    TileRef idx = m_tiles.size() - 1;

    m_names.emplace(tileName, idx);

    return idx;
}

Tile const &TileRenderMap::get(TileRef ref) const
{
    Assert( ref < m_tiles.size() );
    return m_tiles.at(ref);
}

Tile const &TileRenderMap::get(std::string const &name) const
{
    return m_tiles[getRef(name)];
}

TileRef TileRenderMap::getRef(std::string const &name) const
{
    return m_names.at(name);
}
