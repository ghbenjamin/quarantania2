#include <engine/LuaState.h>
#include <fmt/format.h>
#include <utils/Assert.h>

LuaState::LuaState()
{
    m_lua.open_libraries(sol::lib::base);
    m_lua["print"] = []( std::string const& arg ){ std::cout << arg << "\n"; };
}

sol::state& LuaState::state()
{
    return m_lua;
}

void LuaState::runScriptFile(std::string const& path)
{
    try
    {
        m_lua.unsafe_script_file( path );
    }
    catch ( [[maybe_unused]] std::exception const& ex )
    {
        AssertAlwaysMsg( fmt::format( "Error running script file '{}'", path ) );
    }
}
