#pragma once

#include <ui/Element.h>
#include <ui/TextNode.h>

namespace UI
{


class Label : public Element
{
public:
    explicit Label();
    ~Label() override = default;

    void setText( std::string const& text );

private:

    std::shared_ptr<TextNode> m_textNode;

};


}