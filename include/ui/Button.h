#pragma once

#include <ui/Element.h>

namespace UI
{

class Button : public Element
{
public:
    Button( Manager* manager, Element* parent, std::string const& text, std::function<void()> const& callback );
    ~Button() override = default;

private:
    std::function<void()> m_callback;
};

}
