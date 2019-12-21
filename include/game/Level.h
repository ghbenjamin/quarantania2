#pragma once

#include <memory>
#include <set>
#include <vector>

#include <SDL2/SDL.h>

#include <utils/IdPool.h>
#include <ui/Manager.h>
#include <game/Component.h>
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
#include <state/LevelController.h>
#include <systems/System.h>
#include <game/GEvent.h>
#include <game/GEventDefs.h>


// Forward definitions
class RenderInterface;
class InputInterface;
struct IEvent;
struct IEventKeyPress;
struct IEventMouseMove;
struct IEventMouseDown;
struct IEventWindowResize;
class LevelFactory;
class Action;

// New definitions
using EntityCompMap = std::unordered_map<EntityRef, std::shared_ptr<BaseComponent>>;
using ActionPtr = std::shared_ptr<Action>;

class Level
{
public:
    friend class LevelFactory;

    explicit Level(Vector2i size, LevelContextPtr ctx, RandomGenerator const& rg);
    virtual ~Level() = default;

    // Core
    bool input(IEvent &evt);
    void update(uint32_t ticks, InputInterface& iinter, RenderInterface &rInter);

    Grid& grid();
    GEventHub& events();
    UI::Manager& ui();

    // Entity Management

    EntityRef createEntity();
    void deleteEntity(EntityRef ent);
    void disableEntity(EntityRef ent);
    void enableEntity(EntityRef ent);

    void setPlayer( PlayerPtr&& player );
    PlayerPtr& getPlayer();
    bool isPlayer(EntityRef ref) const;

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
            return std::tuple<std::shared_ptr<CT>...>{ getComponents<CT>(entity)... };
        }
    }

    template <typename CT>
    std::shared_ptr<CT> tryGetComponent(const EntityRef ent)
    {
        auto const& mref = mapForComponent<CT>();
        if ( mref.find(ent) != mref.end() )
        {
            return std::static_pointer_cast<CT>(mref.at(ent));
        }
        else
        {
            return std::shared_ptr<CT>();
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

    // Coordinates
    Vector2i worldCoordsToScreen( Vector2i const& world );
    Vector2i screenCoordsToWorld( Vector2i const& screen );
    Vector2i worldCoordsToTile( Vector2i const& world);
    Vector2i screenCoordsToTile( Vector2i const& screen);
    Vector2i tileCoordsToScreen( Vector2i const& tile );
    Vector2i const& bounds() const;
    int tileCount() const;

    // Actions
    std::vector<ActionPtr> actionsForTile(EntityRef actor, Vector2i tile);
    std::vector<ActionPtr> actionsForEntity(EntityRef actor, EntityRef subject);
    std::vector<ActionPtr> actionsForPosition(EntityRef actor, Vector2i position);
    ActionPtr getDefaultAction(EntityRef actor, Vector2i position);

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

    // UI Methods
    void setupUI();
    void updateCamera(uint32_t ticks, InputInterface& iinter, RenderInterface &rInter);
    void render(uint32_t ticks, InputInterface& iinter, RenderInterface &rInter);
    void renderTiles(uint32_t ticks, RenderInterface &rInter);
    void layoutWindows();

private:

    RandomGenerator m_rg;
    Vector2i m_bounds;
    int m_tileCount;
    TileRenderMap m_renderTileMap;
    std::vector<std::vector<TileRef>> m_mapRendering;
    LD::BaseTileMap m_baseTilemap;
    LevelContextPtr m_ctx;
    Grid m_grid;
    GEventHub m_gevents;
    IdPool<EntityRef> m_entityPool;
    EntityFactory m_entFactory;
    std::unordered_map<ComponentId, EntityCompMap> m_components;
    std::vector<SystemPtr> m_systems;
    std::unique_ptr<Player> m_player;
    UI::Manager m_uiManager;
    Camera m_camera;
    LevelControllerPtr m_controller;
};

using LevelPtr = std::unique_ptr<Level>;
