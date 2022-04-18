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

    // Run a script which has already been loaded from disk into the resource manager
    void runLoadedScript( std::string const& key );

    // Run a script directly from disk
    void runScriptFile( std::string const& path );

    // Run a script from a string buffer
    void runScript( std::string const& script );

private:

    // Register our Vector classes, both int and float versions
    template <typename T, char L>
    static void setVector2Type( LuaState& lua );

    // Regiser all of our usertypes
    void addUserTypes();

    sol::state m_lua;
};