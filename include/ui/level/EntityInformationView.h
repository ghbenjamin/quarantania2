#pragma once

#include <ui/lib/Element.h>
#include <engine/Entity.h>

namespace UI
{

class Label;
class HorizontalRule;

class EntityInformationView : public Element
{
public:
    EntityInformationView(Manager* manager, Element* parent);
    ~EntityInformationView() override = default;
    
    void refresh(EntityRef entity);

private:
    
    std::shared_ptr<Label> addSection( );
    
    std::shared_ptr<Label> m_titleLabel;
    std::vector<std::shared_ptr<HorizontalRule>> m_dividers;
    std::vector<std::shared_ptr<Label>> m_sections;
};


}