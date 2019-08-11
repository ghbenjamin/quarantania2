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

    std::string const& label();
private:
    std::string m_label;
};

class ContextMenuSpacer : public Element
{
public:
    ContextMenuSpacer();
    ~ContextMenuSpacer() override = default;
};

}


using ContextMenuList = std::vector<std::string>;
using ContextMenuCallback = std::function<void(std::string const&)>;

class ContextMenu : public Element
{
public:
    explicit ContextMenu( ContextMenuList const& items, ContextMenuCallback callback);
    ~ContextMenu() override = default;

private:
    ContextMenuCallback m_callback;

};

}
