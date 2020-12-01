#pragma once

#include <memory>
#include <set>
#include <unordered_map>

#include <engine/Component.h>
#include <engine/Entity.h>
#include <systems/System.h>
#include <game/EntityFactory.h>
#include <utils/IdPool.h>



// Typedefs
using EntityCompMap = std::unordered_map<EntityRef, std::shared_ptr<BaseComponent>>;

// Forward definitions
class Level;
class InputInterface;

class ECS
{
public:

    ECS(Level* parent);
    ~ECS() = default;


    void update(uint32_t ticks, InputInterface& iinter, RenderInterface &rInter);


    EntityFactory& entityFactory();

    EntityRef createEntity();
    void deleteEntity(EntityRef ent);
    void deleteEntityDelayed(EntityRef ent);

    void entityReady( EntityRef ent );


    template <typename CT, typename... Args>
    std::shared_ptr<CT> addComponent(EntityRef ent, Args&&... args)
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

private:

    // ECS Methods

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

    template< template < typename...> class CTList, typename ...Ts >
    void registerComponentsImpl( CTList<Ts...>&& )
    {
        (registerComponent<Ts>(), ...);
    }

    template <typename CTList>
    void registerComponents()
    {
        registerComponentsImpl( CTList{} );
    }

    template <typename ST>
    void registerSystem()
    {
        static_assert( std::is_base_of_v<System, ST> );

        auto dPtr = std::make_unique<ST>( m_parent );
        auto bPtr = std::unique_ptr<System>( dPtr.release() );
        m_systems.push_back( std::move(bPtr) );
    }

    template< template < typename...> class STList, typename ...Ts >
    void registerSystemsImpl( STList<Ts...>&& )
    {
        (registerSystem<Ts>(), ...);
    }

    template <typename STList>
    void registerSystems()
    {
        registerSystemsImpl( STList{} );
    }


    Level* m_parent;

    IdPool<EntityRef> m_entityPool;
    std::vector<EntityRef> m_delayedDeleteEnts;
    EntityFactory m_entFactory;
    std::vector<SystemPtr> m_systems;
    std::unordered_map<ComponentId, EntityCompMap> m_components;

};