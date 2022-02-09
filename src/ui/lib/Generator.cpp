#include <ui/lib/Generator.h>
#include <ui/lib/Element.h>
#include <ui/lib/Manager.h>
#include <utils/Assert.h>
#include <utils/Memory.h>

#include <rapidxml/rapidxml.hpp>
#include <fmt/format.h>

using namespace UI;


Generator::Generator( Manager *manager )
        : m_manager(manager) {}

std::shared_ptr<Element> Generator::fromXML( std::string const& xmlstr )
{
    std::string localXml = xmlstr;
    rapidxml::xml_document doc;
    doc.parse<0>( localXml.data() );
    
    auto rootNode = doc.first_node( "element" );
    
    auto ptr =  createElement( rootNode, nullptr );
    
    return ptr;
}

std::shared_ptr<Element> Generator::createElement( rapidxml::xml_node<char>* node, Element* parent )
{
    auto attrs = attrsToMap( node );
    std::shared_ptr<Element> out;

    if ( attrs.contains( "type") )
    {
        auto const& nodeType = attrs["type"];
        if ( nodeType == "button" )
        {
            auto btnLabel = attrs["text"];
            auto btnElem =  m_manager->createElement<Button>( parent, btnLabel, [](){} );
            
            if ( attrs.contains("text-size") )
            {
                btnElem->getLabel().setTextSize( std::stoi( attrs["text-size"] ) );
            }
            
            out = btnElem;
        }
        else if ( nodeType == "label" )
        {
        
        }
        else
        {
            AssertAlways();
        }
    }
    else
    {
        out = m_manager->createElement( parent );
    }
    
    
    auto layout = node->first_node("layout");
    if ( layout != nullptr )
    {
        out->setLayout( nodeToLayout(layout, out.get()) );
    }
    
    
    auto preferredSize = node->first_node("preferred-size");
    if ( preferredSize != nullptr )
    {
        auto prefSizeAttrs = attrsToMap( preferredSize );
        out->setPreferredContentSize({
            std::stoi( prefSizeAttrs["width"] ),
            std::stoi( prefSizeAttrs["height"] )
        });
    }
    
    
    auto children = node->first_node("children");
    if ( children != nullptr )
    {
        for ( auto child = children->first_node("element"); child; child = child->next_sibling() )
        {
            createElement( child, out.get() );
        }
    }
    
    out->doLayout();
    return out;
}

std::map<std::string, std::string> Generator::attrsToMap( rapidxml::xml_node<char> *node )
{
    std::map<std::string, std::string> out;
    
    for (auto *attr = node->first_attribute(); attr; attr = attr->next_attribute() )
    {
        out.emplace( attr->name(), attr->value() );
    }
    
    return out;
}

std::unique_ptr<ElementLayout> Generator::nodeToLayout( rapidxml::xml_node<char> *node, Element* parent )
{
    auto attrs = attrsToMap( node );
    
    Assert( attrs.contains("type") );
    auto const& layoutType = attrs["type"];
    
    if ( layoutType == "vertical" )
    {
        int spacing = 0; // Default
        if ( attrs.contains("spacing"))
        {
            spacing = std::stoi( attrs["spacing"] );
        }
        
        HAlignment halign = HAlignment::Left; // Default
        if ( attrs.contains("halign") )
        {
            auto halignStr = attrs["halign"];
            if ( halignStr == "right" )
            {
                halign = HAlignment::Right;
            }
            else if ( halignStr == "centre" )
            {
                halign = HAlignment::Centre;
            }
            else if ( halignStr == "fill" )
            {
                halign = HAlignment::Fill;
            }
        }
        
        return utils::make_unique_with_type<ElementLayout, VerticalLayout>( parent, spacing, halign );
    }
    else if ( layoutType == "horizontal" )
    {
        int spacing = 0; // Default
        if ( attrs.contains("spacing"))
        {
            spacing = std::stoi( attrs["spacing"] );
        }
    
        VAlignment valign = VAlignment::Top; // Default
        if ( attrs.contains("halign") )
        {
            auto valignStr = attrs["valign"];
            if ( valignStr == "right" )
            {
                valign = VAlignment::Centre;
            }
            else if ( valignStr == "centre" )
            {
                valign = VAlignment::Bottom;
            }
            else if ( valignStr == "fill" )
            {
                valign = VAlignment::Fill;
            }
        }
    
        return utils::make_unique_with_type<ElementLayout, HorizontalLayout>( parent, spacing, valign );
    }
    else if ( layoutType == "center" )
    {
        return utils::make_unique_with_type<ElementLayout, CenterLayout>( parent );
    }
    else
    {
        AssertAlwaysMsg( fmt::format( "Unknown layout type '{}'", layoutType ) );
        return std::unique_ptr<ElementLayout>();
    }
}


