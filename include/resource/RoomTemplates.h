#pragma once

#include <string>
#include <utils/Containers.h>
#include <rapidjson/document.h>
#include <regex>
#include <utils/Random.h>
#include <game/Prefab.h>


struct RoomTemplatePrefab
{
    PrefabType type;
    Vector2i position;
};

struct RoomTemplate
{
    std::vector<RoomTemplatePrefab> prefabs;
};


class RoomTemplateManager
{
public:
    RoomTemplateManager() = default;
    ~RoomTemplateManager() = default;

   void loadAllTemplates( std::string const& path );

   RoomTemplate* randomTemplate( Vector2i roomSize, RandomGenerator& rd );

private:
    // Turn "7x5" -> (7, 5)
    Vector2i parseKeyName( std::string const& name );

    std::unordered_multimap<Vector2i, RoomTemplate, Vector2iHash> m_basicTemplates;

    static const inline std::regex KeyParseRegex{"(\\d+)x(\\d+)" };
};