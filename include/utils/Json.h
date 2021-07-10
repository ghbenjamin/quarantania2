#pragma once

#include <nlohmann/json.hpp>

namespace Utils::Json
{

nlohmann::json loadFromPath( std::string const& path );

}