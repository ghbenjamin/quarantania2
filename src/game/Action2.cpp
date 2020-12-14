#include <game/Action2.h>
#include <game/ResourceDatabase.h>

Action2::Action2(const RawActionData &data)
    : m_data(&data)
{

}

std::string const &Action2::getName() const
{
    return m_data->name;
}

std::string const &Action2::getDescription() const
{
    return m_data->description;
}

SpritesheetKey const &Action2::getSprite() const
{
    return m_data->sprite;
}

bool Action2::getProvokes() const
{
    return m_data->provokes;
}
