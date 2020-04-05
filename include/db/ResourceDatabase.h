#pragma once

#include <memory>
#include <string_view>
#include <sqlite3.h>

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

    template <typename Func, typename... Args>
    void getSingleRow( sqlite3_stmt* stmt, Func func, Args... args  )
    {
        int status;
        int idx = 0;

        ( sqlite3_bind_text( stmt, ++idx, std::forward<Args>(args), -1, NULL ), ... );

        status = sqlite3_step( stmt );
        AssertMsg( status == SQLITE_ROW, "SQL query didn't resolve row" );

        func( stmt );

        sqlite3_reset( stmt );
    }

    static std::string getStringColumn( sqlite3_stmt* stmt, int idx );
    static int getIntColumn( sqlite3_stmt* stmt, int idx );

private:
    sqlite3* m_db;

    sqlite3_stmt* m_itemByNameQuery;
    sqlite3_stmt* m_weaponByNameQuery;
    sqlite3_stmt* m_armourByNameQuery;
    sqlite3_stmt* m_creatureByNameQuery;


};