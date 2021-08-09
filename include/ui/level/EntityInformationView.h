#pragma once

#include <ui/lib/Element.h>
#include <ui/lib/Controls.h>
#include <engine/Entity.h>

class Level;

namespace UI
{

enum class EntityInformationSectionType
{
    Meta,
    Stats,
    Statuses,
    Feats
};

class EntityInformationContent : public Element
{
public:
    EntityInformationContent( Manager* manager, Element* parent, Level* level );
    ~EntityInformationContent() override = default;
    
    virtual void refresh(EntityRef entity) = 0;

protected:
    Level* m_level;
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


namespace EntityInfoContent
{

class TaggedLabel : public Label
{
public:
    TaggedLabel( Manager* manager, Element* parent, TextStyle ts );
    ~TaggedLabel() override = default;
    
    void refresh( int value, int modifier );
};




class MetaView : public EntityInformationContent
{
public:
    MetaView(Manager* manager, Element* parent, Level* level);
    void refresh( EntityRef entity ) override;

private:
    std::shared_ptr<Label> m_lEntityType;
};


class StatsView : public EntityInformationContent
{
public:
    StatsView(Manager* manager, Element* parent, Level* level);
    void refresh( EntityRef entity ) override;

private:
    std::shared_ptr<TaggedLabel> m_lStrValue;
    std::shared_ptr<TaggedLabel> m_lDexValue;
    std::shared_ptr<TaggedLabel> m_lConValue;
    std::shared_ptr<TaggedLabel> m_lIntValue;
    std::shared_ptr<TaggedLabel> m_lWisValue;
    std::shared_ptr<TaggedLabel> m_lChaValue;
    std::shared_ptr<TaggedLabel> m_lFortValue;
    std::shared_ptr<TaggedLabel> m_lRefValue;
    std::shared_ptr<TaggedLabel> m_lWillValue;
};

class StatusView : public EntityInformationContent
{
public:
    StatusView(Manager* manager, Element* parent, Level* level);
    void refresh( EntityRef entity ) override;

};


class FeatsView : public EntityInformationContent
{
public:
    FeatsView(Manager* manager, Element* parent, Level* level);
    void refresh( EntityRef entity ) override;

private:
    std::vector<std::shared_ptr<Label>> m_feats;
};



}
}