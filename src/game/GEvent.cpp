#include <game/GEvent.h>

void GEventHub::broadcast(GEvent evt)
{
    auto it_range = m_subs.equal_range( evt.type );

    for (auto it = it_range.first; it != it_range.second; it++)
    {
        it->second->acceptGEvent( evt );
    }
}
