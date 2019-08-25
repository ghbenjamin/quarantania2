#pragma once

#include <string>

#include <game/Entity.h>

struct ImPlayerData
{
    std::string name;
};


class Player
{
public:

    explicit Player( ImPlayerData&& data, EntityRef ref);
    ~Player() = default;

    EntityRef ref();

private:

    EntityRef m_ref;
    const ImPlayerData m_imData;

};

using PlayerPtr = std::unique_ptr<Player>;
