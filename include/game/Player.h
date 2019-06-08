#pragma once

#include <string>

#include <game/Entity.h>

struct ImPlayerData
{
    std::string name;
    EntityRef entity;
};


class Player
{
public:

    explicit Player( ImPlayerData& data );
    ~Player() = default;

private:
    ImPlayerData m_imData;

};