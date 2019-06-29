#pragma once

#include <memory>
#include <set>
#include <vector>

#include <SDL2/SDL.h>

#include <utils/IdPool.h>
#include <game/Component.h>
#include <game/System.h>
#include <game/GEvent.h>
#include <game/Tiles.h>
#include <game/Player.h>
#include <game/LevelData.h>
#include <game/LevelContext.h>
#include <game/LevelGrid.h>
#include <game/EntityFactory.h>

class RenderInterface;
class InputInterface;
struct IEvent;
struct IEventKeyPress;

using EntityCompMap = std::unordered_map<EntityRef, std::shared_ptr<BaseComponent>>;


class Level
{
public:

    explicit Level(ImmutableLevelData&& imd, LevelContextPtr ctx);
    virtual ~Level() = default;

    bool input(IEvent &evt);
    bool handleKeyInput(IEventKeyPress& evt);

    void update(uint32_t ticks, InputInterface& iinter, RenderInterface &rInter);

    GEventHub& events();

    EntityRef createEntity();
    void deleteEntity(EntityRef ent);
    void disableEntity(EntityRef ent);
    void enableEntity(EntityRef ent);

    void setPlayer( std::unique_ptr<Player>&& player );

    template <typename CT, typename... Args>
    void addComponent(EntityRef ent, Args...args)
    {
        auto ptr = std::make_shared<CT>( std::forward<Args>(args)... );
        mapForComponent<CT>()[ent] = std::static_pointer_cast<BaseComponent>(ptr);
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
    decltype(auto) get(const EntityRef entity)
    {
        if constexpr(sizeof...(CT) == 1)
        {
            return (std::static_pointer_cast<CT>(mapForComponent<CT>()[entity]), ...);
        }
        else
        {
            return std::tuple<std::shared_ptr<CT>...>{
                std::static_pointer_cast<CT>(get<CT>(entity))...};
        }
    }

    /**
     * Return all Entities which contain (at least) the specified components.
     */
    template<typename... CT>
    std::vector<EntityRef> having()
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
    decltype(auto) with()
    {
        std::vector<std::tuple<std::shared_ptr<CT>...>> out;

        for ( auto const ent : having<CT...>() )
        {
            out.push_back({ get<CT>(ent) ... });
        }

        return out;
    }


private:

    template <typename CT>
    EntityCompMap& mapForComponent()
    {
//        auto id = Component<CT>::id();
//        auto it = m_components.find(id);
//        return it->second;

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

    void updateCamera(uint32_t ticks, InputInterface& iinter, RenderInterface &rInter);
    void render(uint32_t ticks, InputInterface& iinter, RenderInterface &rInter);
    void renderTiles(uint32_t ticks, RenderInterface &rInter);

    // Player input
    void doMovePlayer( SDL_Keycode kcode );

private:

    const ImmutableLevelData m_imData;
    LevelGrid m_grid;
    LevelContextPtr m_ctx;

    GEventHub m_gevents;

    IdPool<EntityRef> m_entityPool;
    std::unordered_map<ComponentId, EntityCompMap> m_components;
    std::vector<SystemPtr> m_systems;
    std::unique_ptr<Player> m_player;

};

using LevelPtr = std::unique_ptr<Level>;
