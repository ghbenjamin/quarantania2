#include <game/Player.h>
#include <algorithm>
#include <game/ResourceDatabase.h>


PlayerData PCData::generateNewPlayer() const
{
    PlayerData pd;

    pd.sprite = this->sprite;
    pd.name = this->name;
    pd.alignment = this->alignment;
    pd.clazz = this->clazz;
    pd.race = this->race;

    pd.attrStr = this->attrStr;
    pd.attrDex = this->attrDex;
    pd.attrCon = this->attrCon;
    pd.attrInt = this->attrInt;
    pd.attrWis = this->attrWis;
    pd.attrCha = this->attrCha;

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

    pd.baseSpeed = 6;

    return pd;
}