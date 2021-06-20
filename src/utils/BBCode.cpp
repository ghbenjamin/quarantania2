#include <utils/BBCode.h>
#include <regex>
#include <utils/Assert.h>

using namespace BBCode;
using namespace BBCode::Internal;

BBDocument BBCode::parseText( std::string const& text )
{
    BBDocument out;

    // Example markdown:
    // "The <c:#ff0000>quick brown</c> fox jumped <c:blue>over the lazy</c> dog."
    
    std::regex tagOpen { R"(<c:(\S+)>)" };
    std::regex tagClose { R"(</c>)" };
    
    std::sregex_iterator openIt {text.begin(), text.end(), tagOpen };
    std::sregex_iterator closeIt {text.begin(), text.end(), tagClose };
    std::sregex_iterator end;
    
    std::vector<OpenTagPosition> openTags;
    std::vector<EndTagPosition> closeTags;
    
    // Walk over the input string, noting the positions of closing tags and ending tags. We're going to assme that the
    // closing and ending tags match each other in order, so all we have to do is pair them up.
    
    for ( ; openIt != end; openIt++)
    {
        OpenTagPosition tp;
        tp.start = openIt->position();
        tp.end = tp.start + openIt->length();
        tp.value = openIt->str(1);
        
        openTags.push_back(tp);
    }
    
    for ( ; closeIt != end; closeIt++)
    {
        EndTagPosition tp;
        tp.start = closeIt->position();
        tp.end = tp.start + closeIt->length();
        
        closeTags.push_back(tp);
    }
    
    // We're only going to consume strings we created here, so we don't need to handle bad string very gracefully
    AssertMsg( openTags.size() == closeTags.size(), "Open and close tag mismatch" );
    
    // If we have no tags, return the entire text as a single node.
    if ( openTags.size() == 0 )
    {
        out.addNode( text );
        return out;
    }
    
    // Otherwise, create a node for each tagged and untagged region.
    std::size_t lastIdx = 0;
    
    for ( int i = 0; i < (int)openTags.size(); i++ )
    {
        // Add the text region from the last end tag (or start of string) to the beginning of this tag.
        out.addNode( text.substr( lastIdx, openTags[i].start - lastIdx ) );
        
        // Add the tagged region
        out.addNode(
            text.substr( openTags[i].end, closeTags[i].start - openTags[i].end ),
            Colour{ openTags[i].value }
        );
        
        lastIdx = closeTags[i].end;
    }
    
    // Add the region from the end of the last tag to the end of the string.
    out.addNode( text.substr( closeTags.back().end, std::string::npos ) );
    
    return out;
}

BBNode::BBNode( std::string const &text )
: text(text) {}

BBNode::BBNode( std::string const &text, std::optional<Colour> const &colour )
: text(text), colour(colour) {}

BBDocument::Iterator::Iterator( BBDocument const* ptr, std::size_t idx )
    : m_ptr(ptr), m_charIdx(0), m_nodeIdx(0)
{
    for ( ; m_nodeIdx < ptr->m_nodes.size(); m_nodeIdx++ )
    {
        std::size_t nodeSize = ptr->m_nodes[m_nodeIdx].text.size();
        if ( idx >= nodeSize )
        {
            idx -= nodeSize;
        }
        else
        {
            m_charIdx = idx;
            break;
        }
    }
    
    auto& vec =  m_ptr->m_nodes[m_nodeIdx];
    m_curr = std::make_pair( vec.text.at(m_charIdx), vec.colour );
}


bool BBDocument::Iterator::operator==( BBDocument::Iterator const &i )
{
    return m_ptr == i.m_ptr && m_charIdx == i.m_charIdx && m_nodeIdx == i.m_nodeIdx;
}

bool BBDocument::Iterator::operator!=( BBDocument::Iterator const &i )
{
    return !(*this == i);
}

BBDocument::Iterator &BBDocument::Iterator::operator++()
{
    m_charIdx++;
    if ( m_charIdx >=m_ptr->m_nodes[m_nodeIdx].text.size() )
    {
        m_charIdx = 0;
        m_nodeIdx++;
    }
    
    auto& vec =  m_ptr->m_nodes[m_nodeIdx];
    m_curr = std::make_pair( vec.text.at(m_charIdx), vec.colour );
    
    return *this;
}

CharColour BBDocument::Iterator::operator*()
{
    return m_curr;
}

CharColour *BBDocument::Iterator::operator->()
{
    return &m_curr;
}

BBDocument::Iterator BBDocument::begin()
{
    return BBDocument::Iterator(this, 0);
}

BBDocument::Iterator BBDocument::end()
{
    return BBDocument::Iterator(this, m_size);
}

void BBDocument::addNode( BBNode node )
{
    m_nodes.push_back(node);
    m_size += node.text.size();
}

void BBDocument::addNode( std::string const& data, Colour colour )
{
    m_nodes.emplace_back(data, colour );
    m_size += m_nodes.back().text.size();
}

std::vector<BBNode> const &BBDocument::nodes() const
{
    return m_nodes;
}
