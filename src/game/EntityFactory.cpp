#include <game/EntityFactory.h>
#include <game/Level.h>
#include <resource/ResourceManager.h>
#include <rapidjson/document.h>
#include <components/All.h>

EntityFactory::EntityFactory(Level *parent, RandomGenerator* rg )
: m_parent(parent), m_rg(rg)
{
    createPrefabs();
}

std::unique_ptr<Player> EntityFactory::createPlayer(ImPlayerData &data, Vector2i startPos) const
{
    auto eref = m_parent->createEntity();
    auto sprite = ResourceManager::get().getSprite("kenney-chars", "example-char-1");

    m_parent->addComponent<Components::TilePosition>(eref, startPos);
    m_parent->addComponent<Components::Render>(eref, sprite);
    m_parent->addComponent<Components::Collider>(eref);
    m_parent->addComponent<Components::Actor>(eref);

    return std::make_unique<Player>( std::move(data), eref );
}

EntityRef EntityFactory::createPrefabByName(std::string const &name, Vector2i pos) const
{
    auto eref = m_parent->createEntity();
    auto const& prefabComponents = m_prefabs.at(name);

    // All prefabs have a position
    m_parent->addComponent<Components::TilePosition>(eref, pos);

    // Construct the simple components
    for ( auto const& pc : prefabComponents )
    {
        std::visit(PrefabVisitor{m_parent, eref, m_rg}, pc );
    }

    // Construct the components which are specific to specific types of prefab
    if ( m_prefabDecorators.find(name) != m_prefabDecorators.end() )
    {
        auto mptr = m_prefabDecorators.at(name);
        ( this->*mptr ) (eref);
    }

    m_parent->entityReady( eref );

    return eref;
}

void EntityFactory::createPrefabs()
{
    prefabEntrance();
    prefabExit();
    prefabDoor();
    prefabContainer();
    prefabDecor();
}

void EntityFactory::prefabDoor()
{
    PrefabComponent::Render render;
    render.spriteBreakpoints = {2, 1};
    render.sprites = {
        { "kenney-tiles", "door-closed" },
        { "kenney-tiles", "door-barred" },
        { "kenney-tiles", "door-open" }
    };

    PrefabComponent::Collider collider;
    collider.defaultState = false;

    PrefabComponent::Description description;
    description.descriptions = {
        "An open doorway",
        "A closed door"
    };

    PrefabComponent::State state;
    state.states = {
        "open",
        "closed"
    };

    PrefabList prefabList = {
        render, collider, description, state
    };

    m_prefabs.emplace( "door", prefabList );
}

void EntityFactory::prefabExit()
{
    PrefabComponent::Render render;
    render.spriteBreakpoints = {1};
    render.sprites = {
            { "kenney-tiles", "grey-stairs-down" }
    };

    PrefabComponent::Collider collider;
    collider.defaultState = false;

    PrefabComponent::Description description;
    description.descriptions = {
            "Stairs leading up"
    };

    PrefabList prefabList = {
            render, collider, description
    };

    m_prefabs.emplace( "exit", prefabList );
}

void EntityFactory::prefabEntrance()
{
    PrefabComponent::Render render;
    render.spriteBreakpoints = {1};
    render.sprites = {
            { "kenney-tiles", "grey-stairs-up" }
    };

    PrefabComponent::Collider collider;
    collider.defaultState = false;

    PrefabComponent::Description description;
    description.descriptions = {
        "Stairs leading up"
    };

    PrefabList prefabList = {
        render, collider, description
    };

    m_prefabs.emplace( "entrance", prefabList );
}

void EntityFactory::prefabContainer()
{
    PrefabComponent::Render render;
    render.spriteBreakpoints = {11};
    render.sprites = {
        { "kenney-tiles", "bookcase-medium-full"},
        { "kenney-tiles", "bookcase-small-empty"},
        { "kenney-tiles", "bookcase-small-full"},
        { "kenney-tiles", "bookcase-medium-empty"},
        { "kenney-tiles", "bookcase-large-empty"},
        { "kenney-tiles", "bookcase-large-full"},
        { "kenney-tiles", "barrel-open"},
        { "kenney-tiles", "barrel-closed"},
        { "kenney-tiles", "single-wide-drawer" },
        { "kenney-tiles", "double-wide-drawer" },
        { "kenney-tiles", "single-thin-drawer" },
        { "kenney-tiles", "double-thin-drawer" }
    };

    PrefabComponent::Collider collider;
    collider.defaultState = false;

    PrefabComponent::Description description;
    description.descriptions = {
        "A container of some sort"
    };

    PrefabComponent::Container container;

    PrefabList prefabList = {
        render, collider, description, container
    };

    m_prefabs.emplace( "container", prefabList );
}

void EntityFactory::prefabDecor()
{
    PrefabComponent::Render render;
    render.spriteBreakpoints = {6};
    render.sprites = {
        { "kenney-tiles", "bed-made" },
        { "kenney-tiles", "bed-unmade" },
        { "kenney-tiles", "chair-left" },
        { "kenney-tiles", "chair-right" },
        { "kenney-tiles", "fireplace-lit" },
        { "kenney-tiles", "fireplace-out" }
    };

    PrefabComponent::Collider collider;
    collider.defaultState = false;

    PrefabComponent::Description description;
    description.descriptions = {
        "Decor!"
    };

    PrefabList prefabList = {
        render, collider, description
    };

    m_prefabs.emplace( "decor", prefabList );
}


