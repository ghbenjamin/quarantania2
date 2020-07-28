#include <game/EntityFactory.h>
#include <game/Level.h>
#include <resource/ResourceManager.h>
#include <components/All.h>
#include <components/ContainerComponent.h>
#include <game/ResourceDatabase.h>
#include <game/ActionDefs.h>

EntityFactory::EntityFactory(Level *parent)
: m_parent(parent) {}


EntityRef EntityFactory::createPlayer(PlayerData const& data, Vector2i pos) const
{
    auto eref = m_parent->createEntity();

    auto sprite = ResourceManager::get().getSprite(data.sprite);
    sprite.setRenderLayer(RenderLayer::Actor);

    m_parent->addComponent<PositionComponent>(eref, pos);
    m_parent->addComponent<RenderComponent>(eref, sprite);
    m_parent->addComponent<ColliderComponent>(eref, false, true);
    m_parent->addComponent<PCComponent>(eref);

    auto cContainer = m_parent->addComponent<ContainerComponent>(eref);
    auto cActor = m_parent->addComponent<ActorComponent>(eref, data);

    m_parent->entityReady(eref);

    for ( auto const& iname : data.startingHeldItems )
    {
        cContainer->items.push_back( Item::fromName( iname ) );
    }

    for ( auto const& iname : data.startingEquippedItems )
    {
        auto ptr = Item::fromName( iname );
        cActor->equipItem( ptr->getEquipSlot(), ptr );
    }

    return eref;
}

EntityRef EntityFactory::createEnemy(std::string const &name, Vector2i pos) const
{
    auto eref = m_parent->createEntity();
    auto creatureData = ResourceDatabase::instance().creatureFromName( name );

    auto sprite = ResourceManager::get().getSprite( creatureData.sprite );
    sprite.setRenderLayer(RenderLayer::Actor);

    m_parent->addComponent<PositionComponent>(eref, pos);
    m_parent->addComponent<RenderComponent>(eref, sprite);
    m_parent->addComponent<ColliderComponent>(eref, false, true);
    m_parent->addComponent<DescriptionComponent>( eref, creatureData.name, "Creature", creatureData.description );

    auto actComp = m_parent->addComponent<ActorComponent>(eref, std::move(creatureData));

    m_parent->entityReady(eref);
    return eref;
}

EntityRef EntityFactory::createItem(std::string const &name, Vector2i pos) const
{
    ItemPtr item = Item::fromName( name );
    return createItem( item, pos );
}

EntityRef EntityFactory::createItem(std::shared_ptr<Item> item, Vector2i pos) const
{
    auto eref = m_parent->createEntity();

    auto sprite = ResourceManager::get().getSprite( item->getSprite() );
    sprite.setRenderLayer(RenderLayer::Entity);

    m_parent->addComponent<PositionComponent>(eref, pos);
    m_parent->addComponent<RenderComponent>(eref, sprite);
    m_parent->addComponent<ColliderComponent>(eref, false, false);
    m_parent->addComponent<ItemComponent>(eref, item);
    m_parent->addComponent<DescriptionComponent>( eref, item->getName(), "Item", item->getDescription() );

    m_parent->entityReady(eref);
    return eref;
}

EntityRef EntityFactory::createObject(std::string const &ptype, Vector2i pos) const
{
    auto eref = m_parent->createEntity();
    auto objData = ResourceDatabase::instance().objectFromName( ptype );

    auto sprite = ResourceManager::get().getSprite( objData.sprites[0] );
    sprite.setRenderLayer(RenderLayer::Entity);

    m_parent->addComponent<PositionComponent>(eref, pos);
    m_parent->addComponent<RenderComponent>(eref, sprite);

    if ( objData.type == "door" )
    {
        m_parent->addComponent<RenderComponent>(eref, sprite);
        m_parent->addComponent<ColliderComponent>(eref, true, true);
        m_parent->addComponent<OpenableComponent>(eref);
    }
    else if ( objData.type == "level_entrance" )
    {
        m_parent->addComponent<ColliderComponent>(eref, true, true);
    }
    else if ( objData.type == "level_exit" )
    {
        m_parent->addComponent<ColliderComponent>(eref, true, true);

        auto actions = m_parent->addComponent<ActionComponent>(eref);
        actions->actions.push_back( std::make_shared<ExitLevelAction>(m_parent) );
    }
    else if ( objData.type == "container" )
    {
    }
    else if ( objData.type == "decor" )
    {
    }
    else
    {
        AssertAlways();
    }

    m_parent->entityReady(eref);
    return eref;
}

EntityRef EntityFactory::createDecor(Vector2i pos, SpritesheetKey const& key)
{
    auto eref = m_parent->createEntity();

    auto sprite = ResourceManager::get().getSprite( key );
    sprite.setRenderLayer(RenderLayer::Entity);

    m_parent->addComponent<PositionComponent>(eref, pos);
    m_parent->addComponent<RenderComponent>(eref, sprite);

    m_parent->entityReady(eref);
    return eref;
}
