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
        return PrefabType::Cont_Bookcase_Large;
    }
    else if ( name == "entrance" )
    {
        return PrefabType::Stairs_Up;
    }
    else if ( name == "exit" )
    {
        return PrefabType::Stairs_Down;
    }
    else if ( name == "decor" )
    {
        return PrefabType::Decor_Bed;
    }
    else if ( name == "Door" )
    {
        return PrefabType::Door;
    }
    else if ( name == "Door_Locked" )
    {
        return PrefabType::Door_Locked;
    }
    else if ( name == "Stairs_Up" )
    {
        return PrefabType::Stairs_Up;
    }
    else if ( name == "Stairs_Down" )
    {
        return PrefabType::Stairs_Down;
    }
    else if ( name == "Cont_Bookcase_Small" )
    {
        return PrefabType::Cont_Bookcase_Small;
    }
    else if ( name == "Cont_Bookcase_Large" )
    {
        return PrefabType::Cont_Bookcase_Large;
    }
    else if ( name == "Cont_Chest" )
    {
        return PrefabType::Cont_Chest;
    }
    else if ( name == "Cont_Chest_Locked" )
    {
        return PrefabType::Cont_Chest_Locked;
    }
    else if ( name == "Cont_Barrel" )
    {
        return PrefabType::Cont_Barrel;
    }
    else if ( name == "Cont_Crate" )
    {
        return PrefabType::Cont_Crate;
    }
    else if ( name == "Decor_Bed" )
    {
        return PrefabType::Decor_Bed;
    }
    else if ( name == "Decor_Fireplace" )
    {
        return PrefabType::Decor_Fireplace;
    }
    else if ( name == "Decor_Chair" )
    {
        return PrefabType::Decor_Chair;
    }
    else
    {
        Logging::log( "ERROR: Unknown name :{}\n");
        AssertAlways();

        // Silence warning
        return PrefabType::Decor_Bed;
    }
}


PrefabObjSprite::PrefabObjSprite(SpritesheetKey sprite)
        : m_sprite( std::move(sprite) ) { }


void PrefabObjects::Door::generate(Level *level, EntityRef entity)
{
    auto sprite = ResourceManager::get().getSprite(m_sprite);
    sprite.setRenderLayer(RenderLayer::Entity);
    
    level->addComponent<RenderComponent>(entity, sprite);
    level->addComponent<ColliderComponent>(entity, true, true);
    level->addComponent<OpenableComponent>(entity);
}

void PrefabObjects::Exit::generate(Level *level, EntityRef entity)
{
    auto sprite = ResourceManager::get().getSprite(m_sprite);
    sprite.setRenderLayer(RenderLayer::Entity);
    level->addComponent<RenderComponent>(entity, sprite);

    level->addComponent<ColliderComponent>(entity, true, true);

    auto actions = level->addComponent<ActionComponent>(entity);
    actions->actions.push_back( std::make_shared<ExitLevelAction>(level) );
}

void PrefabObjects::Entrance::generate(Level *level, EntityRef entity)
{
    auto sprite = ResourceManager::get().getSprite(m_sprite);
    sprite.setRenderLayer(RenderLayer::Entity);
    level->addComponent<RenderComponent>(entity, sprite);

    level->addComponent<ColliderComponent>(entity, true, true);
}

void PrefabObjects::Container::generate(Level *level, EntityRef entity)
{
    auto sprite = ResourceManager::get().getSprite(m_sprite);
    sprite.setRenderLayer(RenderLayer::Entity);
    level->addComponent<RenderComponent>(entity, sprite);

    // TODO Re-enable container collision when level gen stops generating impossible levels
    // level->addComponent<ColliderComponent>(entity, true, true);
}

void PrefabObjects::Decor::generate(Level *level, EntityRef entity)
{
    auto sprite = ResourceManager::get().getSprite(m_sprite);
    sprite.setRenderLayer(RenderLayer::Entity);
    level->addComponent<RenderComponent>(entity, sprite);

    // TODO Re-enable decor collision when level gen stops generating impossible levels
    //level->addComponent<ColliderComponent>(entity, true, true);
}
