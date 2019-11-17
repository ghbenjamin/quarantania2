#include <game/EntityFactory.h>
#include <game/Level.h>
#include <resource/ResourceManager.h>
#include <rapidjson/document.h>
#include <utils/Json.h>

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
        std::visit( Prefab::Visitor{m_parent, eref, m_rg}, pc );
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
    Prefab::Component::Render render;
    render.renderStates = 1;
    render.spriteBreakpoints = {2, 1};
    render.sprites = {
        { "kenney-tiles", "door-closed" },
        { "kenney-tiles", "door-barred" },
        { "kenney-tiles", "door-open" }
    };

    Prefab::Component::Collider collider;
    collider.defaultState = false;

    Prefab::Component::Description description;
    description.descriptions = {
        "An open doorway",
        "A closed door"
    };

    Prefab::Component::State state;
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
    Prefab::Component::Render render;
    render.renderStates = 1;
    render.spriteBreakpoints = {1};
    render.sprites = {
            { "kenney-tiles", "grey-stairs-down" }
    };

    Prefab::Component::Collider collider;
    collider.defaultState = false;

    Prefab::Component::Description description;
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
    Prefab::Component::Render render;
    render.renderStates = 1;
    render.spriteBreakpoints = {1};
    render.sprites = {
            { "kenney-tiles", "grey-stairs-up" }
    };

    Prefab::Component::Collider collider;
    collider.defaultState = false;

    Prefab::Component::Description description;
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
    Prefab::Component::Render render;
    render.renderStates = 1;
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

    Prefab::Component::Collider collider;
    collider.defaultState = false;

    Prefab::Component::Description description;
    description.descriptions = {
        "A container of some sort"
    };

    Prefab::Component::Container container;

    PrefabList prefabList = {
        render, collider, description, container
    };

    m_prefabs.emplace( "container", prefabList );
}

void EntityFactory::prefabDecor()
{
    Prefab::Component::Render render;
    render.renderStates = 1;
    render.spriteBreakpoints = {6};
    render.sprites = {
        { "kenney-tiles", "bed-made" },
        { "kenney-tiles", "bed-unmade" },
        { "kenney-tiles", "chair-left" },
        { "kenney-tiles", "chair-right" },
        { "kenney-tiles", "fireplace-lit" },
        { "kenney-tiles", "fireplace-out" }
    };

    Prefab::Component::Collider collider;
    collider.defaultState = false;

    Prefab::Component::Description description;
    description.descriptions = {
        "Decor!"
    };

    PrefabList prefabList = {
        render, collider, description
    };

    m_prefabs.emplace( "decor", prefabList );
}


Prefab::Visitor::Visitor(Level* level, EntityRef ref, RandomGenerator* rg)
: m_ref(ref), m_level(level), m_rg(rg) {}

void Prefab::Visitor::operator()(Component::Render const& obj) const
{
    // TODO: something more sophisticated than randomly picking

    auto begin_it = obj.sprites.begin();
    auto end_it = obj.sprites.begin();
    std::advance(end_it, obj.spriteBreakpoints[0]);

    auto it = randomElement(begin_it, end_it, *m_rg);
    auto sprite = ResourceManager::get().getSprite(*it);

    m_level->addComponent<Components::Render>(m_ref, sprite);
}

void Prefab::Visitor::operator()(Component::State const& obj) const
{
    m_level->addComponent<Components::FixedState>(m_ref, obj.states);
}

void Prefab::Visitor::operator()(Component::Collider const& obj) const
{
    m_level->addComponent<Components::Collider>(m_ref);
}

void Prefab::Visitor::operator()(Component::Container const& obj) const
{
}

void Prefab::Visitor::operator()(Component::Description const& obj) const
{
    m_level->addComponent<Components::Description>(m_ref, obj.descriptions);
}