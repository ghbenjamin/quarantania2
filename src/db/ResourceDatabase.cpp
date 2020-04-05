#include <db/ResourceDatabase.h>
#include <exception>
#include <utils/Logging.h>
#include <game/Items.h>
#include <utils/Assert.h>

ResourceDatabase::ResourceDatabase()
{
    int ierr = sqlite3_open_v2( "../resource/data/psrd_data.db", &m_db, SQLITE_OPEN_READONLY, NULL );
    if ( ierr != SQLITE_OK )
    {
        Logging::log( "FATAL ERROR: Unable to open content database" );
        std::terminate();
    }

    sqlite3_prepare_v2( m_db, "SELECT * FROM item_data WHERE item_data.item_name=?", -1, &m_itemByNameQuery, NULL );
    sqlite3_prepare_v2( m_db, "SELECT * FROM weapon_data WHERE weapon_data.item_name=?", -1, &m_weaponByNameQuery, NULL );
    sqlite3_prepare_v2( m_db, "SELECT * FROM armour_data WHERE armour_data.item_name=?", -1, &m_armourByNameQuery, NULL );
    sqlite3_prepare_v2( m_db, "SELECT * FROM creature_data WHERE creature_data.name=?", -1, &m_creatureByNameQuery, NULL );
}

ResourceDatabase::~ResourceDatabase()
{
    sqlite3_close( m_db );
}

RawItemData ResourceDatabase::itemFromName(std::string_view name)
{
    RawItemData data;
    data.name = std::string(name);

    getSingleRow( m_itemByNameQuery, [&data]( sqlite3_stmt* stmt ){

        data.weight = getIntColumn( stmt, 1 );
        data.value = getIntColumn( stmt, 2 );
        data.description = getStringColumn(stmt, 3);
        data.aura_strength = getStringColumn(stmt, 4);
        data.aura_value = getStringColumn(stmt, 5);
        data.slot = getStringColumn(stmt, 6);
        data.item_type = getStringColumn(stmt, 7);
        data.sprite = { "dawnlike_items", "LongWep_028" }; // TODO DEBUG

    }, data.name.c_str() );

    return std::move(data);
}


RawWeaponData ResourceDatabase::weaponFromName(std::string_view name)
{
    RawWeaponData data;

    data.item_name = std::string(name);

    getSingleRow( m_weaponByNameQuery, [&data]( sqlite3_stmt* stmt ){

        data.weapon_class = getStringColumn(stmt, 1);
        data.damage = getStringColumn(stmt, 2);
        data.proficiency = getStringColumn(stmt, 3);
        data.crit_lower = getIntColumn(stmt, 4);
        data.crit_mult = getIntColumn(stmt, 5);
        data.damage_type = getStringColumn(stmt, 6);
        data.specials = getStringColumn(stmt, 7);

    }, data.item_name.c_str() );

    return std::move(data);
}


std::string ResourceDatabase::getStringColumn(sqlite3_stmt* stmt, int idx)
{
    const char* ptr = reinterpret_cast<const char *>(sqlite3_column_text(stmt, idx));
    auto len = sqlite3_column_bytes(stmt, idx);

    return std::string( ptr, len );
}

int ResourceDatabase::getIntColumn(sqlite3_stmt* stmt, int idx)
{
    return sqlite3_column_int( stmt, idx );
}

RawCreatureData ResourceDatabase::creatureFromName(std::string_view name)
{
    RawCreatureData data;
    data.name = std::string(name);

    getSingleRow( m_creatureByNameQuery, [&data]( sqlite3_stmt* stmt ){

        data.description = getStringColumn(stmt, 1);
        data.xp = getIntColumn(stmt, 2);
        data.hp = getIntColumn(stmt, 3);
        data.alignment = getStringColumn(stmt, 4);
        data.size = getStringColumn(stmt, 5);
        data.creature_type = getStringColumn(stmt, 6);
        data.creature_subtype = getStringColumn(stmt, 7);

    }, data.name.c_str() );

    return std::move(data);
}

RawArmourData ResourceDatabase::armourFromName(std::string_view name)
{
    RawArmourData data;
    data.item_name = std::string(name);

    getSingleRow( m_armourByNameQuery, [&data]( sqlite3_stmt* stmt ){

      data.arcaneFailureChance = getIntColumn(stmt, 1);
      data.armourBonus = getIntColumn(stmt, 2);
      data.shieldBonus = getIntColumn(stmt, 3);
      data.armourCheck = getIntColumn(stmt, 4);
      data.maxDex = getIntColumn(stmt, 5);
      data.speed20 = getIntColumn(stmt, 6);
      data.speed30 = getIntColumn(stmt, 7);
      data.armour_type = getStringColumn(stmt, 8);

    }, data.item_name.c_str() );

    return std::move(data);
}

