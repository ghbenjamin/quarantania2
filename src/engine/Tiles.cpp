#include <engine/Tiles.h>
#include <utils/Assert.h>
#include <resource/ResourceManager.h>

Tile& Tileset::get(TileRef ref)
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

std::shared_ptr<Texture> Tileset::getTexture()
{
    return m_texture;
}

void Tileset::setTexture(std::shared_ptr<Texture> tex)
{
    m_texture = tex;
}
