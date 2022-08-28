#include <ui/lib/Generator.h>

#include <fmt/format.h>

#include <ui/lib/Element.h>
#include <ui/lib/Manager.h>
#include <ui/lib/ScrollHolder.h>
#include <utils/Assert.h>
#include <utils/Memory.h>
#include <resource/ResourceManager.h>

using namespace UI;


Generator::Generator( Manager *manager, LuaState& lua )
        : m_manager(manager), m_lua(lua) {}
        

std::shared_ptr<Element> Generator::fromLua( std::string const &script, Element *existing )
{
    sol::table luagen = m_lua.runLoadedScript( script );
    
    return createElement( luagen, nullptr, existing );
}

std::shared_ptr<Element> Generator::createElement( sol::table const& node, Element *parent, Element *existing )
{
    std::shared_ptr<Element> created = std::shared_ptr<Element>();
    Element* target;
    
    // If an existing pointer has been specified, just annotate the existing element rather than making a new one
    if ( existing != nullptr  )
    {
        target = existing;
    }
    else
    {
        created = nodeToElement( node, parent );
        target = created.get();
    }
    
    auto idNode = node["id"];
    if ( idNode != sol::nil )
    {
        target->setId( idNode.get<std::string>() );
    }
    
    auto paddingNode = node["padding"];
    if ( paddingNode != sol::nil )
    {
        target->setPadding( paddingNode.get<int>() );
    }
    
    auto layoutNode = node["layout"];
    if ( layoutNode != sol::nil )
    {
        target->setLayout( nodeToLayout(layoutNode, target) );
    }
    
    auto backgroundNode = node["background"];
    if ( backgroundNode != sol::nil )
    {
        nodeToBackground( backgroundNode, target );
    }
    
    auto preferredSizeNode = node["preferred_size"];
    if ( preferredSizeNode != sol::nil )
    {
        target->setPreferredContentSize({
            preferredSizeNode["width"].get<int>(),
            preferredSizeNode["height"].get<int>()
        });
    }
    
    auto childrenNode = node["children"];
    if ( childrenNode != sol::nil )
    {
        for ( auto const&[k, v] : childrenNode.get<sol::table>() )
        {
            createElement( v.as<sol::table>(), target );
        }
    }
    
    target->doLayout();
    return created;
}

std::unique_ptr<ElementLayout> Generator::nodeToLayout( sol::table const &node, Element *parent )
{
    std::string const& layoutType = node["type"];
    
    if ( layoutType == "vertical" )
    {
        // Default values
        HAlignment halign = HAlignment::Left;
        int spacing = 0;
        
        auto const& spacingNode = node["spacing"];
        if ( spacingNode != sol::nil )
        {
            spacing = spacingNode;
        }
        
        auto const& halignNode = node["halign"];
        if ( halignNode != sol::nil )
        {
            std::string const& halignStr = halignNode;
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
        VAlignment valign = VAlignment::Top; // Default
    
        auto const& spacingNode = node["spacing"];
        if ( spacingNode != sol::nil )
        {
            spacing = spacingNode;
        }
        
        auto const& valignNode = node["valign"];
        if ( valignNode != sol::nil )
        {
            std::string const& valignStr = valignNode;
            if ( valignStr == "top" )
            {
                valign = VAlignment::Top;
            }
            else if ( valignStr == "centre" )
            {
                valign = VAlignment::Centre;
            }
            else if ( valignStr == "bottom" )
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

std::shared_ptr<Element> Generator::nodeToElement( sol::table const& node, Element *parent )
{
    auto typeElem = node["type"];
    if ( typeElem != sol::nil )
    {
        std::string const& typeval = typeElem.get<std::string>();
        
        if ( typeval == "button" )
        {
            auto btnLabel = node["text"].get<std::string>();
            auto btnElem =  m_manager->createElement<Button>( parent, btnLabel );
    
            if ( node["text_size"] != sol::nil )
            {
                btnElem->getLabel().setTextSize( node["text_size"].get<int>() );
            }
            
            return btnElem;
        }
        else if ( typeval == "label" )
        {
            auto labelElem =  m_manager->createElement<Label>( parent );
            
            if ( node["text_size"] != sol::nil )
            {
                labelElem->setTextSize( node["text_size"].get<int>() );
            }
    
            if ( node["text_colour"] != sol::nil )
            {
                labelElem->setTextColour( Colour(node["text_colour"].get<std::string>()) );
            }
    
            if ( node["text"] != sol::nil )
            {
                labelElem->setText( node["text"].get<std::string>() );
            }
            
            return labelElem;
        }
        else if ( typeval == "scroll" )
        {
            return m_manager->createElement<ScrollHolder>( parent );
        }
        else if ( typeval == "horizontal_rule" )
        {
            Colour hrule_colour;
            if ( node["colour"] != sol::nil )
            {
                hrule_colour = Colour(node["colour"].get<std::string>());
            }
            else
            {
                hrule_colour = Colour::Green;
            }
            
            return m_manager->createElement<HorizontalRule>( parent, hrule_colour );
        }
    }
    
    // Default case
    return m_manager->createElement( parent );
}

void Generator::nodeToBackground( sol::table const &node, Element *parent )
{
    std::string const bgType = node["type"];
    
    if ( bgType == "ninepatch" )
    {
        auto const& patch = ResourceManager::get().getNinePatch( node["name"].get<std::string>() ).getPatch();
        parent->setBackground( patch );
        parent->setBorderWidth( patch.getBorderWidth() );
    }
}