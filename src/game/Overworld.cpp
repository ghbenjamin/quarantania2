#include <game/Overworld.h>
#include <graphics/RenderInterface.h>

Overworld::Overworld( std::shared_ptr<OverworldData> const &data, RandomState* randomState )
    : m_data(data), m_randomState(randomState), m_gevents(&m_animation)
{
    if (data->currentLocation == -1)
    {
        m_allowedNextLocs = data->rootNodes;
    }
    else
    {
    }
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
    return m_data->locations;
}

std::unordered_multimap<int, int> const &Overworld::connections()
{
    return m_data->connections;
}

Vector2i const &Overworld::gridSize() const
{
    return m_data->gridSize;
}

GameEventHub &Overworld::events()
{
    return m_gevents;
}

int Overworld::currentLocation() const
{
    return m_data->currentLocation;
}

std::unordered_set<int> const &Overworld::nextLocations() const
{
    return m_allowedNextLocs;
}

std::shared_ptr<OverworldData> Overworld::data()
{
    return m_data;
}


