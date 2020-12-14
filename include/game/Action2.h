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


    virtual GridRegion possibleTiles(EntityRef source) const = 0;


protected:
    Level* m_level;

private:
    const RawActionData m_data;

};


class MoveAction : public Action
{
public:
    MoveAction(Level* level, std::string const& id, int range);
    ~MoveAction() override = default;

    GridRegion possibleTiles(EntityRef source) const override;

private:
    int m_range;
};