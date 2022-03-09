#include <ui/lib/LuaInterface.h>
#include <ui/lib/Manager.h>

using namespace UI;

LuaInterface::LuaInterface( Manager *manager )
    : m_manager(manager)
{
    m_state.open_libraries(sol::lib::base);
}
