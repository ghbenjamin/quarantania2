#include <game/Overworld.h>
#include <graphics/RenderInterface.h>

Overworld::Overworld( OverworldData const &data, RandomState* randomState )
    : m_data(data), m_randomState(randomState), m_gevents(&m_animation)
{
}

bool Overworld::input( IEvent &evt )
{
    return false;
}

void Overworld::update( uint32_t ticks, InputInterface &iinter, RenderInterface &rInter )
{
    rInter.setCameraOffset({0.0f, 0.0f});

    m_animation.update(ticks, iinter, rInter);
    
    m_gevents.pollAllEvents();
}

std::vector<OverworldLocation> const &Overworld::locations()
{
    return m_data.locations;
}

std::unordered_multimap<Vector2i, Vector2i, Vector2iHash> const &Overworld::connections()
{
    return m_data.connections;
}

Vector2i const &Overworld::gridSize() const
{
    return m_data.gridSize;
}

GameEventHub &Overworld::events()
{
    return m_gevents;
}


