#pragma once

#include <ui/Element.h>
#include <ui/TextNode.h>

namespace UI
{

namespace Internal
{

class ContextMenuItem : public Element
{
public:
    explicit ContextMenuItem( std::string const& label );
    ~ContextMenuItem() override = default;
};

class ContextMenuSpacer : public Element
{
public:
    ContextMenuSpacer();
    ~ContextMenuSpacer() override = default;
};

}


using ContextMenuList = std::vector<std::string>;

class ContextMenu : public Element
{
public:
    explicit ContextMenu( ContextMenuList const& items );
    ~ContextMenu() override = default;

private:

};

}
