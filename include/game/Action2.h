#pragma once

#include <string>
#include <resource/Spritesheet.h>

class RawActionData;

class Action2
{
public:
    Action2( RawActionData const& data );
    virtual ~Action2() = default;

    std::string const& getName() const;
    std::string const& getDescription() const;
    SpritesheetKey const& getSprite() const;
    bool getProvokes() const;

protected:


private:
    RawActionData const* m_data;

};







namespace Actionz
{

class MoveAction : public Action2
{
};

class StepAction : public Action2
{
};

class StrikeAction : public Action2
{
};

}

