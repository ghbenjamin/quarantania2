#pragma once

#include <memory>
#include <string_view>

#include <game/RawData.h>
#include <utils/Assert.h>
#include <utils/Random.h>

class RandomInterface;

class ResourceDatabase
{
public:

    static ResourceDatabase& instance()
    {
        static ResourceDatabase s;
        return s;
    }

    ~ResourceDatabase() = default;

    ItemData itemFromName( std::string_view name );
    WeaponData weaponFromName (std::string_view name );
    ArmourData armourFromName( std::string_view name );
    CreatureData creatureFromName( std::string_view name );
    RawPlayerRaceData playerRaceFromName( std::string_view name );
    RawPlayerClassData playerClassFromName( std::string_view name );
    RawObjectData objectFromName( std::string_view name );
    ActionData actionFromId(std::string_view id );
    FeatData featFromId( std::string_view id );

private:
    ResourceDatabase();

    void loadAllCreatureData();
    void loadAllItemData();
    void loadAllPlayerData();
    void loadAllObjectData();
    void loadAllActionData();
    void loadAllFeatData();

    std::vector<CreatureData> m_creatureData;
    std::vector<ItemData> m_itemData;
    std::vector<WeaponData> m_weaponData;
    std::vector<ArmourData> m_armourData;
    std::vector<RawPlayerClassData> m_playerClassData;
    std::vector<RawPlayerRaceData> m_playerRaceData;
    std::vector<RawObjectData> m_objectData;
    std::vector<ActionData> m_actionData;
    std::vector<FeatData> m_featData;
};