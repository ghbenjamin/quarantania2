#include <game/Overworld.h>

Overworld::Overworld( OverworldData const &data, RandomGenerator const& rg )
    : m_data(data), m_rg(rg), m_gevents(&m_animation) {}

bool Overworld::input( IEvent &evt )
{
    return false;
}

void Overworld::update( uint32_t ticks, InputInterface &iinter, RenderInterface &rInter )
{
    m_animation.update(ticks, iinter, rInter);
    
    m_gevents.pollAllEvents();
}

std::vector<OverworldLocation> const &Overworld::locations()
{
    return m_data.locations;
}

std::unordered_map<int, int> const &Overworld::connections()
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


