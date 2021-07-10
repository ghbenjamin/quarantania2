#pragma once

#include <rapidjson/document.h>

namespace Utils::Json
{

using Document = rapidjson::Document;
using Object = rapidjson::Value;
using Value = rapidjson::Value;

Document loadFromPath( std::string const& path );

}