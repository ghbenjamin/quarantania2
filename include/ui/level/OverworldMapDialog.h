#pragma once

#include <ui/lib/Element.h>

class Level;
class Overworld;

namespace UI
{

class OverworldMapDialog : public Element
{
public:
    OverworldMapDialog(Manager* manager, Element* parent, Level* level, Overworld* overworld);
    ~OverworldMapDialog() override = default;

private:
    Level* m_level;
    Overworld* m_overworld;
};

}