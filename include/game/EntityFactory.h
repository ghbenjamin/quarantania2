#pragma once

#include <variant>
#include <resource/Spritesheet.h>
#include <game/Entity.h>
#include <utils/Containers.h>
#include <game/Player.h>
#include <utils/Random.h>

class Level;

namespace Prefab
{
namespace Component
{
    struct Render
    {
        int renderStates = -1;
        std::vector<SpritesheetKey> sprites;
    };

    struct State
    {

    };

    struct Collider
    {
        bool defaultState = false;
    };

    struct Container
    {

    };
}

    class Visitor
    {
    public:
        explicit Visitor(Level* level, EntityRef ref, RandomGenerator* rg);
        ~Visitor() = default;

        void operator()(Component::Render const& obj) const;
        void operator()(Component::State const& obj) const;
        void operator()(Component::Collider const& obj) const;
        void operator()(Component::Container const& obj) const;

    private:
        EntityRef m_ref;
        Level* m_level;
        RandomGenerator* m_rg;
    };

}

using PrefabVariant = std::variant
    <
        Prefab::Component::Render,
        Prefab::Component::State,
        Prefab::Component::Collider,
        Prefab::Component::Container
    >;

using PrefabList = std::vector<PrefabVariant>;



class EntityFactory
{
public:

    explicit EntityFactory( Level* parent, RandomGenerator* rg );
    ~EntityFactory() = default;

    void loadAllPrefabs( std::string const& path );

    std::unique_ptr<Player> createPlayer( ImPlayerData & data, Vector2i startPos ) const;
    EntityRef createPrefabByName(std::string const &name, Vector2i pos) const;

    EntityRef debugHighlight( Vector2i pos, std::string const& tile) const;

private:

    Level* m_parent;
    RandomGenerator* m_rg;
    std::unordered_map<std::string, PrefabList> m_prefabs;

};