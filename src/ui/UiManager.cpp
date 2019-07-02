#include <ui/UiManager.h>

#include <graphics/RenderInterface.h>
#include <game/InputInterface.h>
#include <utils/Logging.h>

using namespace UI;

bool UiManager::input(IEvent &evt)
{
    return false;
}

void UiManager::update(uint32_t ticks, InputInterface &iinter, RenderInterface &rInter)
{
    for ( auto const& r: m_roots )
    {
        r->update(ticks, iinter, rInter);
    }
}
