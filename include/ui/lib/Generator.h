#pragma once

#include <memory>
#include <string>
#include <map>


namespace rapidxml
{
    template<class Ch> class xml_node;
}

namespace UI
{

class Element;
class Manager;
class ElementLayout;

class Generator
{
public:
    Generator( Manager* manager );
    
    std::shared_ptr<Element> fromXML( std::string const& xmlstr );
    
private:
    
    std::shared_ptr<Element> createElement( rapidxml::xml_node<char>* node, Element* parent );
    std::map<std::string, std::string> attrsToMap( rapidxml::xml_node<char>* node );
    std::unique_ptr<ElementLayout> nodeToLayout( rapidxml::xml_node<char>* node, Element* parent );
    
    Manager* m_manager;

};


}