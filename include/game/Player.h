#pragma once

#include <string>
#include <memory>
#include <vector>

#include <game/Entity.h>

struct PlayerData
{
    std::string name;
    std::string race;

    int attr_str;
    int attr_dex;
    int attr_con;
    int attr_int;
    int attr_wis;
    int attr_cha;

    std::vector<std::string> startingHeldItems;
};


class Player
{
public:

    explicit Player(PlayerData const& data, EntityRef ref);
    ~Player() = default;

    EntityRef ref();

private:

    EntityRef m_ref;
    PlayerData m_imData;

};

using PlayerPtr = std::unique_ptr<Player>;
