#include <systems/System.h>


System::System(Level *parent) : m_level(parent)
{
    // Implement me in children
}

void System::update(uint32_t ticks, RenderInterface &rInter)
{
    // Implement me in children
    // By default, do nothing
}