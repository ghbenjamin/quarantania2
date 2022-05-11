#pragma once

#include <memory>
#include <string>
#include <map>

#include <engine/LuaState.h>


namespace UI
{

class Element;
class Manager;
class ElementLayout;

class Generator
{
public:
    Generator( Manager* manager, LuaState& lua );
    
    std::shared_ptr<Element> fromLua( std::string const& script, Element* existing = nullptr );
    
private:
    
    std::shared_ptr<Element> createElement( sol::table const& node, Element* parent, Element* existing = nullptr );
    std::shared_ptr<Element> nodeToElement( sol::table const& node, Element* parent );
    std::unique_ptr<ElementLayout> nodeToLayout( sol::table const& node, Element* parent );
    void nodeToBackground( sol::table const& node, Element* parent );
    
    Manager* m_manager;
    LuaState& m_lua;
};


}