#pragma once

#include <functional>
#include <ui/lib/Element.h>
#include <ui/lib/Controls.h>

namespace UI
{

class ContextMenuItem : public Element
{
public:
    explicit ContextMenuItem(Manager* manager, Element* parent, std::string const& label, std::size_t idx );
    ~ContextMenuItem() override = default;

    std::string const& label();
    std::size_t index();

private:
    std::string m_label;
    std::size_t m_idx;
};

class ContextMenuSpacer : public Element
{
public:
    ContextMenuSpacer(Manager* manager, Element* parent);
    ~ContextMenuSpacer() override = default;
};



using ContextMenuList = std::vector<std::string>;
using ContextMenuCallback = std::function<void(std::size_t)>;

class ContextMenu : public Element
{
public:
    explicit ContextMenu( Manager* manager, Element* parent, ContextMenuList const& items, ContextMenuCallback callback);
    ~ContextMenu() override = default;

private:
    ContextMenuCallback m_callback;

};

}
