#include <game/Overworld.h>

Overworld::Overworld( OverworldData const &data, RandomGenerator const& rg )
    : m_data(data), m_rg(rg)
{
}

bool Overworld::input( IEvent &evt )
{
    return false;
}

void Overworld::update( uint32_t ticks, InputInterface &iinter, RenderInterface &rInter )
{

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


