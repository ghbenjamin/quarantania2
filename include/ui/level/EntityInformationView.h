#pragma once

#include <ui/lib/Element.h>
#include <engine/Entity.h>

class Level;

namespace UI
{

class Label;
class HorizontalRule;



class EntityInformationContent : public Element
{
public:
    EntityInformationContent( Manager* manager, Element* parent );
    ~EntityInformationContent() override = default;
    
    virtual void refresh(EntityRef entity) = 0;
};


class EntityInformationSection : public Element
{
public:
    EntityInformationSection( Manager* manager, Element* parent, std::string title, std::shared_ptr<EntityInformationContent> content );
    ~EntityInformationSection() override = default;
    
    void refresh(EntityRef entity);

protected:
    std::shared_ptr<Label> m_title;
    std::shared_ptr<EntityInformationContent> m_content;
};


class EntityInformationView : public Element
{
public:
    EntityInformationView(Manager* manager, Element* parent, Level* level);
    ~EntityInformationView() override = default;
    
    void refresh(EntityRef entity);

private:
    Level* m_level;
    std::shared_ptr<Label> m_nameLabel;
    std::vector<std::shared_ptr<EntityInformationSection>> m_sections;
};


}