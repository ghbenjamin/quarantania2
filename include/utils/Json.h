#pragma once

#include <nlohmann/json.hpp>



namespace utils::json
{

using object = nlohmann::json;

nlohmann::json loadFromPath( std::string const& path );

}