#pragma once

#include <vector>
#include <memory>
#include <optional>

#include <utils/Colour.h>

namespace BBCode
{
    namespace Internal
    {
        struct OpenTagPosition
        {
            std::size_t start;
            std::size_t end;
            std::string value;
        };
        
        struct EndTagPosition
        {
            std::size_t start;
            std::size_t end;
        };
    }
    
    
struct BBNode
{
    BBNode( std::string const &text );
    BBNode( std::string const &text, std::optional<Colour> const &colour );
    
    std::string text;
    std::optional<Colour> colour;
};

using CharColour = std::pair<char, std::optional<Colour>>;

class BBDocument
{
public:
    BBDocument() = default;
    std::vector<BBNode> nodes;
    
    class Iterator
    {
    public:
        using value_type = CharColour;
        using reference = CharColour&;
        using iterator_category=std::input_iterator_tag;
        using pointer = CharColour*;
        using difference_type = void;
        
        Iterator( BBDocument const* ptr, std::size_t idx );
        ~Iterator() = default;
    
        bool operator==(Iterator const& i);
        bool operator!=(Iterator const& i);
        Iterator& operator++();
        CharColour operator*();
        CharColour* operator->();
        
        BBDocument const* m_ptr;
        CharColour m_curr;
        std::size_t m_nodeIdx;
        std::size_t m_charIdx;
    };
    
    Iterator begin();
    Iterator end();
};

/**
 * Accept a string which represents text with colour tagging and parse it into a collection of substring-colour pairs.
 */
BBDocument parseText( std::string const& text );

}