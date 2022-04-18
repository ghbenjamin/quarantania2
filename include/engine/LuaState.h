#pragma once

#include <memory>

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

class LuaState
{
public:
    LuaState();
    ~LuaState() = default;

    sol::state& state();

    void runLoadedScript( std::string const& key );

    void runScriptFile( std::string const& path );
    void runScript( std::string const& script );

private:

    template <typename T, char L>
    static void setVector2Type( LuaState& lua );

    void addUserTypes();

    sol::state m_lua;
};