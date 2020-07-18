#include <engine/Tiles.h>
#include <utils/Assert.h>
#include <resource/ResourceManager.h>

Tile const &Tileset::get(TileRef ref) const
{
    return m_tiles.at(ref);
}

TileRef Tileset::addTile(SpritesheetKey const& key)
{
    Tile t;
    t.sprite = ResourceManager::get().getSprite(key);
    t.sprite.setRenderLayer(RenderLayer::Tiles);

    m_tiles.push_back(t);
    return m_tiles.size() - 1;
}