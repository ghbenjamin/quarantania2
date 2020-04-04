#pragma once

#include <memory>
#include <string_view>
#include <sqlite3.h>

class Item;

class ResourceDatabase
{
public:
    ~ResourceDatabase();

    static ResourceDatabase& instance()
    {
        static ResourceDatabase s;
        return s;
    }


    std::shared_ptr<Item> itemFromName( std::string_view name );


private:
    ResourceDatabase();

private:
    sqlite3* m_db;
    sqlite3_stmt* m_itemByNameQuery;


};