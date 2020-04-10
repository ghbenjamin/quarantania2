#pragma once

#include <memory>
#include <string_view>

#include <db/RawData.h>
#include <utils/Assert.h>

class ResourceDatabase
{
public:
    ~ResourceDatabase();

    static ResourceDatabase& instance()
    {
        static ResourceDatabase s;
        return s;
    }

    RawItemData itemFromName( std::string_view name );
    RawWeaponData weaponFromName( std::string_view name );
    RawArmourData armourFromName( std::string_view name );
    RawCreatureData creatureFromName( std::string_view name );

private:
    ResourceDatabase();

    void loadAllCreatureData( );
    void loadAllItemData( );

private:
    std::vector<RawCreatureData> m_creatureData;
    std::vector<RawItemData> m_itemData;
    std::vector<RawWeaponData> m_weaponData;
    std::vector<RawArmourData> m_armourData;
};