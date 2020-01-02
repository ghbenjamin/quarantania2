#include <game/Prefab.h>
#include <game/Level.h>
#include <resource/ResourceManager.h>
#include <components/All.h>
#include <actions/ActionDefs.h>

PrefabType prefabTypeFromName(std::string const& name)
{
    if ( name == "door" )
    {
        return PrefabType::Door;
    }
    else if ( name == "container" )
    {
        return PrefabType::Container;
    }
    else if ( name == "entrance" )
    {
        return PrefabType::Entrance;
    }
    else if ( name == "exit" )
    {
        return PrefabType::Exit;
    }
    else if ( name == "decor" )
    {
        return PrefabType::Decor;
    }

    else
    {
        Logging::log( "ERROR: Unknown name :{}\n");
        AssertAlways();

        // Silence warning
        return PrefabType::Entrance;
    }
}

PrefabObjs::Door::Door(SpritesheetKey sprite)
    : m_sprite(sprite)
{
}

void PrefabObjs::Door::generate(Level *level, EntityRef entity)
{
    auto sprite = ResourceManager::get().getSprite(m_sprite);
    sprite.setRenderLayer(RenderLayer::Entity);
    
    level->addComponent<Components::Render>(entity, sprite);
    level->addComponent<Components::Collider>(entity, true, true);
    level->addComponent<Components::Openable>(entity);
}

PrefabObjs::Exit::Exit(SpritesheetKey sprite)
    : m_sprite(sprite)
{
}

void PrefabObjs::Exit::generate(Level *level, EntityRef entity)
{
    auto sprite = ResourceManager::get().getSprite(m_sprite);
    sprite.setRenderLayer(RenderLayer::Entity);
    level->addComponent<Components::Render>(entity, sprite);

    level->addComponent<Components::Collider>(entity, true, true);

    auto actions = level->addComponent<Components::Action>(entity);
    actions->actions.push_back( std::make_shared<ExitLevelAction>(level) );
}


PrefabObjs::Entrance::Entrance(SpritesheetKey sprite)
    : m_sprite(sprite)
{
}

void PrefabObjs::Entrance::generate(Level *level, EntityRef entity)
{
    auto sprite = ResourceManager::get().getSprite(m_sprite);
    sprite.setRenderLayer(RenderLayer::Entity);
    level->addComponent<Components::Render>(entity, sprite);

    level->addComponent<Components::Collider>(entity, true, true);
}

PrefabObjs::Container::Container(SpritesheetKey sprite)
    : m_sprite(sprite)
{
}

void PrefabObjs::Container::generate(Level *level, EntityRef entity)
{
    auto sprite = ResourceManager::get().getSprite(m_sprite);
    sprite.setRenderLayer(RenderLayer::Entity);
    level->addComponent<Components::Render>(entity, sprite);

    // TODO Re-enable container collision when level gen stops generating impossible levels
    // level->addComponent<Components::Collider>(entity, true, true);
}

PrefabObjs::Decor::Decor(SpritesheetKey sprite)
    : m_sprite(sprite)
{
}

void PrefabObjs::Decor::generate(Level *level, EntityRef entity)
{
    auto sprite = ResourceManager::get().getSprite(m_sprite);
    sprite.setRenderLayer(RenderLayer::Entity);
    level->addComponent<Components::Render>(entity, sprite);

    // TODO Re-enable decor collision when level gen stops generating impossible levels
    //level->addComponent<Components::Collider>(entity, true, true);
}
