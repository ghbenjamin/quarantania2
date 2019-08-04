#pragma once

#include <ui/Element.h>
#include <ui/TextNode.h>

namespace UI
{


class ContextMenuItem : public Element
{
public:
    ContextMenuItem( std::string const& label );
    ~ContextMenuItem() = default;
};



class ContextMenu : public Element
{
public:

    ContextMenu();
    ~ContextMenu() override = default;

private:

};

}
