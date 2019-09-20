#include <systems/System.h>


System::System(Level *parent) : m_level(parent)
{}

void System::update(uint32_t ticks, RenderInterface &rInter)
{}