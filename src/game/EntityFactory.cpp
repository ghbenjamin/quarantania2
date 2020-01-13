#include <game/EntityFactory.h>
#include <game/Level.h>
#include <resource/ResourceManager.h>
#include <rapidjson/document.h>
#include <components/All.h>

EntityFactory::EntityFactory(Level *parent, RandomGenerator* rg )
: m_parent(parent), m_rg(rg)
{
    m_weaponManager.loadAllData();
    m_enemyManager.loadAllData();

    addPrefabType<PrefabObjects::Door>(PrefabType::Door, SpritesheetKey{"dawnlike_objects", "Door_001"});
    addPrefabType<PrefabObjects::Door>(PrefabType::Door_Locked, SpritesheetKey{"dawnlike_objects", "Door_005"});
    addPrefabType<PrefabObjects::Entrance>(PrefabType::Stairs_Up, SpritesheetKey{"dawnlike_objects", "Door_037"});
    addPrefabType<PrefabObjects::Exit>(PrefabType::Stairs_Down, SpritesheetKey{"dawnlike_objects", "Door_038"});
    addPrefabType<PrefabObjects::Container>(PrefabType::Cont_Bookcase_Small, SpritesheetKey{"dawnlike_objects", "Decor_038"});
    addPrefabType<PrefabObjects::Container>(PrefabType::Cont_Bookcase_Large, SpritesheetKey{"dawnlike_objects", "Decor_039"});
    addPrefabType<PrefabObjects::Decor>(PrefabType::Decor_Bed, SpritesheetKey{"dawnlike_objects", "Decor_073"});
}

EntityRef EntityFactory::createPrefab(PrefabType ptype, Vector2i pos) const
{
    auto eref = m_parent->createEntity();
    auto it = m_prefabs.find(ptype);

    // TODO Do we always want a tile position?
    m_parent->addComponent<Components::TilePosition>(eref, pos);

    Assert(it != m_prefabs.end() );

    it->second->generate(m_parent, eref);

    m_parent->entityReady( eref );

    return eref;
}

PlayerPtr EntityFactory::createPlayer(ImPlayerData &data, Vector2i startPos) const
{
    auto eref = m_parent->createEntity();

    // TODO Don't hardcode the player appearance
    auto sprite = ResourceManager::get().getSprite("dawnlike_chars", "Player_001");
    sprite.setRenderLayer(RenderLayer::Actor);

    m_parent->addComponent<Components::TilePosition>(eref, startPos);
    m_parent->addComponent<Components::Render>(eref, sprite);
    m_parent->addComponent<Components::Collider>(eref, false, true);
    m_parent->addComponent<Components::Actor>(eref);

    m_parent->entityReady(eref);

    return std::make_unique<Player>( std::move(data), eref );
}

EntityRef EntityFactory::createEnemy(std::string const &name, Vector2i pos) const
{
    auto eref = m_parent->createEntity();

    Enemy enemy = m_enemyManager.createEnemy(name);

    auto sprite = ResourceManager::get().getSprite(enemy.sprite());
    sprite.setRenderLayer(RenderLayer::Actor);

    m_parent->addComponent<Components::TilePosition>(eref, pos);
    m_parent->addComponent<Components::Render>(eref, sprite);
    m_parent->addComponent<Components::Collider>(eref, false, true);
    m_parent->addComponent<Components::Actor>(eref);

    m_parent->entityReady(eref);
    return eref;
}


