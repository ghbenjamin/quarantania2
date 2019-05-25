#pragma once

#include <rapidjson/document.h>

namespace JsonUtils
{

rapidjson::Document loadFromPath( std::string const& path );

}