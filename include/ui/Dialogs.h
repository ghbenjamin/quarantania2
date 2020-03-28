#pragma once
#include <ui/Element.h>

namespace UI
{

class Dialog : public Element
{
public:
    Dialog( Manager* manager, Element* parent, std::string const& title, std::string const& contents, int maxWidth = -1, bool isModal = false );
    ~Dialog() override = default;

protected:
    std::string m_title;
    std::string m_contents;
    int m_maxWidth;
    bool m_isModal;
};




}