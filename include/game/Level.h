#pragma once

#include <memory>
#include <set>
#include <vector>

#include <SDL2/SDL.h>

#include <utils/IdPool.h>
#include <ui/UiManager.h>
#include <game/Component.h>
#include <game/System.h>
#include <game/GEvent.h>
#include <game/Tiles.h>
#include <game/Player.h>
#include <game/LevelData.h>
#include <game/LevelContext.h>
#include <game/EntityFactory.h>
#include <game/GridFeature.h>
#include <game/Rules.h>
#include <game/Grid.h>
#include <game/Camera.h>

class RenderInterface;
class InputInterface;
struct IEvent;
struct IEventKeyPress;
struct IEventMouseMove;
struct IEventClick;
class LevelFactory;

using EntityCompMap = std::unordered_map<EntityRef, std::shared_ptr<BaseComponent>>;


class Level
{
public:
    friend class LevelFactory;

    explicit Level(Vector2i size, LevelContextPtr ctx);
    virtual ~Level() = default;

    bool input(IEvent &evt);
    void update(uint32_t ticks, InputInterface& iinter, RenderInterface &rInter);

    Vector2i const& bounds() const;
    int tileCount() const;

    Grid& grid();
    GEventHub& events();

    EntityRef createEntity();
    void deleteEntity(EntityRef ent);
    void disableEntity(EntityRef ent);
    void enableEntity(EntityRef ent);

    void setPlayer( std::unique_ptr<Player>&& player );

    template <typename CT, typename... Args>
    std::shared_ptr<CT> addComponent(EntityRef ent, Args...args)
    {
        auto ptr = std::make_shared<CT>( std::forward<Args>(args)... );
        mapForComponent<CT>()[ent] = std::static_pointer_cast<BaseComponent>(ptr);

        return ptr;
    }

    template <typename CT>
    void deleteComponent(EntityRef ent)
    {
        auto& map = mapForComponent<CT>();
        auto it = map.find(ent);
        if ( it != map.end() )
        {
            map.erase(it);
        }
    }

    template<typename... CT>
    decltype(auto) getComponents(const EntityRef entity)
    {
        if constexpr(sizeof...(CT) == 1)
        {
            return (std::static_pointer_cast<CT>(mapForComponent<CT>()[entity]), ...);
        }
        else
        {
            return std::tuple<std::shared_ptr<CT>...>{
                std::static_pointer_cast<CT>(getComponents<CT>(entity))...};
        }
    }

    /**
     * Return all Entities which contain (at least) the specified components.
     */
    template<typename... CT>
    std::vector<EntityRef> entitiesHaving()
    {
        std::set<EntityRef> matching;
        std::set<EntityRef> mbuffer;
        std::set<EntityRef> inter;

        auto typeids = { Component<CT>::id()... };
        bool first = true;

        for ( auto const& ti : typeids )
        {
            if (first)
            {
                first = false;
                for ( auto const&[ent, comp] : m_components[ti] )
                {
                    matching.insert( ent );
                }
            }
            else
            {
                for ( auto const&[ent, comp] : m_components[ti] )
                {
                    mbuffer.insert( ent );
                }

                inter.clear();
                std::set_intersection( matching.begin(), matching.end(),
                                       mbuffer.begin(), mbuffer.end(), std::inserter(inter, inter.begin()) );

                std::swap(matching, inter);
            }
        }

        std::vector<EntityRef> out{ matching.begin(), matching.end() };
        return out;
    }

    /**
     *  Find all entities which have the specified components, and find and return pointers to
     *  each of those components.
     */
    template<typename... CT>
    decltype(auto) entitiesWith()
    {
        std::vector<std::tuple<std::shared_ptr<CT>...>> out;

        for ( auto const ent : entitiesHaving<CT...>() )
        {
            out.push_back({getComponents<CT>(ent) ... });
        }

        return out;
    }

    template<typename...CT>
    bool entityHas( EntityRef ent )
    {
        auto typeids = { Component<CT>::id()... };
        for ( auto const ti: typeids)
        {
            auto const& ecm = m_components[ti];
            auto it = ecm.find( ent );
            if ( it == ecm.end() )
            {
                return false;
            }
        }

        return true;
    }

    void entityReady( EntityRef ent )
    {
        m_gevents.broadcast<GEvents::EntityReady>( ent );
    }


private:

    template <typename CT>
    EntityCompMap& mapForComponent()
    {
        return m_components.at( Component<CT>::id() );
    }

    template <typename CT>
    void registerComponent()
    {
        static_assert( std::is_base_of_v<BaseComponent, CT> );

        m_components[Component<CT>::id()] = EntityCompMap();
    }

    template <typename ST>
    void registerSystem()
    {
        static_assert( std::is_base_of_v<System, ST> );

        auto dPtr = std::make_unique<ST>( this );
        auto bPtr = std::unique_ptr<System>( dPtr.release() );
        m_systems.push_back( std::move(bPtr) );
    }

    bool handleKeyInput(IEventKeyPress& evt);
    bool handleMouseMoveInput(IEventMouseMove evt);
    bool handleMouseClickInput(IEventClick evt);

    void updateCamera(uint32_t ticks, InputInterface& iinter, RenderInterface &rInter);
    void render(uint32_t ticks, InputInterface& iinter, RenderInterface &rInter);
    void renderTiles(uint32_t ticks, RenderInterface &rInter);

    // Player input
    void doMovePlayer( SDL_Keycode kcode );

private:

    Vector2i m_bounds;
    int m_tileCount;
    
    TileRenderMap m_renderTileMap;
    std::vector<std::vector<TileRef>> m_mapRendering;
    LD::BaseTileMap m_baseTilemap;

    Vector2i m_entranceTile;
    Vector2i m_exitTile;

    LevelContextPtr m_ctx;

    Grid m_grid;
    GEventHub m_gevents;

    IdPool<EntityRef> m_entityPool;
    EntityFactory m_entFactory;

    std::unordered_map<ComponentId, EntityCompMap> m_components;
    std::vector<SystemPtr> m_systems;
    std::unique_ptr<Player> m_player;

    UI::UiManager m_uiManager;
    Camera m_camera;

};

using LevelPtr = std::unique_ptr<Level>;
