#pragma once

#include <string>
#include <resource/Spritesheet.h>
#include <game/RawData.h>
#include <engine/Entity.h>
#include <utils/GridUtils.h>

class Level;

class Action
{
public:
    Action(Level* level, std::string const& id);
    virtual ~Action() = default;

    std::string const& getName() const;
    std::string const& getDescription() const;
    SpritesheetKey const& getSprite() const;
    bool getProvokes() const;
    bool isEnabled() const;

    void setEnabled(bool val);

protected:
    Level* m_level;

private:
    const RawActionData m_data;
    bool m_enabled;
};


class MoveAction : public Action
{
public:
    MoveAction(Level* level, std::string const& id, int range);
    ~MoveAction() override = default;

private:
    int m_range;
};


class AttackAction : public Action
{
public:
    AttackAction(Level* level, std::string const& id);
    ~AttackAction() override = default;

private:
};
