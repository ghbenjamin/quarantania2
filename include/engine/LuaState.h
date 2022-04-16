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

    void runScriptFile( std::string const& path );

private:
    sol::state m_lua;
};