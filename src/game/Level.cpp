#include <game/Level.h>

Level::Level()
{

}

bool Level::input(SDL_Event &evt)
{
    return false;
}

void Level::update(uint32_t ticks, RenderInterface &rInterface)
{
    m_ecs.update(ticks, rInterface);
}

const Vector2i &Level::getSize() const
{
    return m_size;
}

void Level::setSize(const Vector2i &size)
{
    m_size = size;
}
