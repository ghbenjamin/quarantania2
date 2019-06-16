#include <game/EntityFactory.h>
#include <game/ECS.h>
#include <resource/ResourceManager.h>

EntityFactory::EntityFactory(ECS *parent)
: m_parent(parent)
{
}


std::unique_ptr<Player> EntityFactory::createPlayer(ImPlayerData &data, Vector2i startPos) const
{
    auto eref = m_parent->createEntity();

    m_parent->addComponent<Components::TilePosition>(eref, startPos);

    auto sprite = ResourceManager::get().getResource<SpritesheetResource>( "kenney-chars" )
        ->get()->spriteFromName( "example-char-1" );
    m_parent->addComponent<Components::Render>(eref, sprite);

    m_parent->addComponent<Components::Collider>(eref);


    return std::make_unique<Player>( std::move(data), eref );
}

