#pragma once

#include <string>
#include <memory>
#include <vector>

#include <game/Entity.h>
#include <game/Defines.h>

struct PlayerData
{
    std::string name;
    std::string race;

    std::vector<std::string> startingHeldItems;
    std::vector<std::string> startingEquippedItems;
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
