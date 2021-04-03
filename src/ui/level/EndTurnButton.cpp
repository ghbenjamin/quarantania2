#include <ui/level/EndTurnButton.h>
#include <ui/lib/Manager.h>
#include <game/Level.h>


UI::EndTurnButton::EndTurnButton(UI::Manager *manager, UI::Element *parent)
        : Button(manager, parent, "End Turn", [this](){ this->onClick(); } )
{
    setPadding(4);
    setId("end-turn-button");
}

void UI::EndTurnButton::onClick()
{
    // End the player turn on click
    manager()->level()->events().broadcast<GameEvents::TurnChange>(false);
}
