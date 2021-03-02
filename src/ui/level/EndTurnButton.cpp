#include <ui/level/EndTurnButton.h>

UI::EndTurnButton::EndTurnButton(UI::Manager *manager, UI::Element *parent)
        : Button(manager, parent, "End Turn", [this](){ this->onClick(); } )
{
    setBackground(Colour::Grey);
    setPadding(4);
    setId("end-turn-button");
}

void UI::EndTurnButton::onClick()
{
}
