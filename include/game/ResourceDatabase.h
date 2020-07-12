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

    RawItemData itemFromName( std::string_view name );
    RawWeaponData weaponFromName( std::string_view name );
    RawArmourData armourFromName( std::string_view name );
    RawCreatureData creatureFromName( std::string_view name );
    RawPlayerRaceData playerRaceFromName( std::string_view name );
    RawPlayerClassData playerClassFromName( std::string_view name );
    RawObjectData objectFromName( std::string_view name );
    RawRoomTemplateData randomRoomTemplate( Vector2i size, RandomInterface& ri );

    static Alignment parseAlignmentFromStr( std::string_view sv );
    static EquipSlot parseEquipSlotFromStr( std::string_view sv );
    static ArmourType parseArmourTypeFromStr( std::string_view sv );

private:
    ResourceDatabase();

    void loadAllCreatureData( );
    void loadAllItemData( );
    void loadAllPlayerData( );
    void loadAllObjectData( );
    void loadAllRoomTemplateData( );

private:
    std::vector<RawCreatureData> m_creatureData;
    std::vector<RawItemData> m_itemData;
    std::vector<RawWeaponData> m_weaponData;
    std::vector<RawArmourData> m_armourData;
    std::vector<RawPlayerClassData> m_playerClassData;
    std::vector<RawPlayerRaceData> m_playerRaceData;
    std::vector<RawObjectData> m_objectData;

    std::unordered_multimap<Vector2i, RawRoomTemplateData, Vector2iHash> m_roomTemplateData;
};