#include <ui/level/EventLog.h>
#include <ui/lib/Manager.h>
#include <resource/ResourceManager.h>

using namespace UI;


EventLog::EventLog( Manager *manager, Element *parent )
    : TextLog(manager, parent)
{
    setPreferredContentSize({350, 150});
    
    auto const& patch = ResourceManager::get().getNinePatch( "simple-border" ).getPatch();
    setBackground( patch );
    setBorderWidth( patch.getBorderWidth() );
    
    setId("main-text-log");
}