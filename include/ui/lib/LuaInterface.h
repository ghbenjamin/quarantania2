#pragma once

#include <sol/sol.hpp>

namespace UI
{

class Manager;

class LuaInterface
{
public:
    LuaInterface( Manager* manager );
    ~LuaInterface() = default;
    
private:
    Manager* m_manager;
    sol::state m_state;
};


}