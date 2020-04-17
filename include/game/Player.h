#pragma once

#include <string>
#include <memory>
#include <vector>

#include <game/Entity.h>
#include <game/Defines.h>


// Data given by the player in the level gen menu to construct a new starting player
struct PlayerGenData
{
    std::string name;
    std::string race;
    std::string clazz;
    Alignment alignment;

    int attrStr, attrDex, attrCon, attrInt, attrWis, attrCha;
};


// Complete description of an active player - this is the data which will be carried
// from level to level
struct PlayerData
{
    std::string name;
    std::string race;
    std::string clazz;
    Alignment alignment;

    int level;
    int currXP;

    int maxHP;
    int currHP;

    int attrStr, attrDex, attrCon, attrInt, attrWis, attrCha;
    int saveFort, saveRef, saveWill;

    int baseSpeed;
    int bab;

    std::vector<std::string> startingHeldItems;
    std::vector<std::string> startingEquippedItems;
};


class Player
{
public:

    static PlayerData generateNewPlayer( PlayerGenData const& pgd );

    explicit Player(PlayerData const& data, EntityRef ref);
    ~Player() = default;

    EntityRef ref();
    PlayerData& data();

private:

    EntityRef m_ref;
    PlayerData m_pData;

};

using PlayerPtr = std::shared_ptr<Player>;
