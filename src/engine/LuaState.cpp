#include <engine/LuaState.h>

#include <fmt/format.h>

#include <resource/ResourceManager.h>
#include <game/Level.h>
#include <ui/lib/Manager.h>
#include <utils/Assert.h>

LuaState::LuaState()
{
    m_lua.open_libraries(sol::lib::base);
    m_lua["print"] = []( std::string const& arg ){ std::cout << arg << "\n"; };

    addUserTypes();
}

sol::state& LuaState::state()
{
    return m_lua;
}

sol::object LuaState::runScriptFile(std::string const& path)
{
    try
    {
        return m_lua.unsafe_script_file( path );
    }
    catch ( [[maybe_unused]] std::exception const& ex )
    {
        AssertAlwaysMsg( fmt::format( "Error running script file '{}'", path ) );
        std::terminate;
    }
}

sol::object LuaState::runScript( std::string const& script )
{
    try
    {
        return m_lua.unsafe_script( script );
    }
    catch ( [[maybe_unused]] std::exception const& ex )
    {
        AssertAlwaysMsg( "Error running script" );
        std::terminate();
    }
}

void LuaState::addUserTypes()
{
    LuaState::setVector2Type<int, 'i'>( *this );
    LuaState::setVector2Type<float, 'f'>( *this );

    Level::setLuaType( *this );

    UI::Manager::setLuaType( *this );
    UI::Element::setLuaType( *this );
}

template <typename T, char L>
void LuaState::setVector2Type(LuaState &lua)
{
    auto userType = lua.state().new_usertype<Vector2<T>>(
            std::string( "Vector2" ) + L,
            sol::constructors<Vector2<T>(T, T)>()
    );

    userType["x"] = sol::property( sol::resolve<T() const>(&Vector2<T>::x), sol::resolve<void(T)>(&Vector2<T>::x) );
    userType["y"] = sol::property( sol::resolve<T() const>(&Vector2<T>::y), sol::resolve<void(T)>(&Vector2<T>::y) );
}

sol::object LuaState::runLoadedScript( std::string const &key )
{
    auto& data = ResourceManager::get().getLuaScript( key );
    return runScript( data.data() );
}


template void LuaState::setVector2Type<int, 'i'>(LuaState& lua);
template void LuaState::setVector2Type<float, 'f'>(LuaState& lua);

