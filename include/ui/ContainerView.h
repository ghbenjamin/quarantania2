#pragma once

#include <ui/Element.h>
#include <game/Entity.h>

namespace UI
{

// A view into a container of items, e.g. a chest or the inventory of an actor
class ContainerView : public Element
{

public:
    ContainerView();
    ~ContainerView() override = default;

private:

    EntityRef m_container;
    Vector2i m_viewportSize;

};

}