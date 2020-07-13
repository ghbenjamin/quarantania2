#pragma once

#include <string>








class TiledMapLoader
{
public:
    TiledMapLoader() = default;
    ~TiledMapLoader() = default;

    void load( std::string const& path );
};