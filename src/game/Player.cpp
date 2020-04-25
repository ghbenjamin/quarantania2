#include <game/Player.h>
#include <algorithm>
#include <db/ResourceDatabase.h>

Player::Player(PlayerData const& data, EntityRef ref)
: m_pData(data), m_ref(ref) {}

EntityRef Player::ref()
{
    return m_ref;
}

PlayerData Player::generateNewPlayer(PlayerGenData const &pgd)
{
    PlayerData pd;

    pd.sprite = { "dawnlike_chars", "Player_001" };
    pd.name = pgd.name;
    pd.alignment = pgd.alignment;
    pd.clazz = pgd.clazz;
    pd.race = pgd.race;

    pd.attrStr = pgd.attrStr;
    pd.attrDex = pgd.attrDex;
    pd.attrCon = pgd.attrCon;
    pd.attrInt = pgd.attrInt;
    pd.attrWis = pgd.attrWis;
    pd.attrCha = pgd.attrCha;

    pd.level = 1;
    pd.currXP = 0;

    auto raceData = ResourceDatabase::instance().playerRaceFromName( pd.race );
    auto classData = ResourceDatabase::instance().playerClassFromName( pd.clazz );

    pd.baseSpeed = raceData.baseSpeed;

    pd.saveFort = classData.fortByLevel[0];
    pd.saveRef = classData.refByLevel[0];
    pd.saveWill = classData.willByLevel[0];
    pd.bab = classData.babByLevel[0];

    pd.maxHP = classData.hitDie;
    pd.currHP = pd.maxHP;

    return pd;
}

PlayerData &Player::data()
{
    return m_pData;
}
