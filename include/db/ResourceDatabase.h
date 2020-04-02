#pragma once

#include <sqlite3.h>

class ResourceDatabase
{
public:
    ~ResourceDatabase();

    static ResourceDatabase& instance()
    {
        static ResourceDatabase s;
        return s;
    }

private:
    ResourceDatabase();

private:
    sqlite3* m_db;
};