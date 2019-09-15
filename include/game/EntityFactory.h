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
        std::vector<int> spriteBreakpoints;
    };

    struct State
    {
        std::vector<std::string> states;
    };

    struct Collider
    {
        bool defaultState = false;
    };

    struct Container
    {

    };

    struct Description
    {
        std::vector<std::string> descriptions;
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
        void operator()(Component::Description const& obj) const;

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
        Prefab::Component::Container,
        Prefab::Component::Description
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

private:

    void createEntrance( EntityRef ref ) const;
    void createExit( EntityRef ref ) const;
    void createDoor( EntityRef ref ) const;


private:

    Level* m_parent;
    RandomGenerator* m_rg;
    std::unordered_map<std::string, PrefabList> m_prefabs;
    std::unordered_map<std::string, void(EntityFactory::*)(EntityRef) const> m_prefabDecorators;

};