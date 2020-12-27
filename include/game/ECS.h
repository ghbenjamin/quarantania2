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

    // Update all of our entities which require per-frame updates, e.g. animation and rendering
    void update(uint32_t ticks, InputInterface& iinter, RenderInterface &rInter);

    // Factory for creating specific entities & components
    EntityFactory& entityFactory();

    // Create a new entity (containing no components)
    EntityRef createEntity();

    // Delete the specified entity and all associated components
    void deleteEntity(EntityRef ent);

    // Delete the specified entity at the beginning of the next frame
    void deleteEntityDelayed(EntityRef ent);

    // Signal that the specified entity has finished construction and is ready to be used
    void entityReady( EntityRef ent );

    // Construct the specified component and attach it to the specified entity
    template <typename CT, typename... Args>
    std::shared_ptr<CT> addComponent(EntityRef ent, Args&&... args)
    {
        auto ptr = std::make_shared<CT>( std::forward<Args>(args)... );
        mapForComponent<CT>()[ent] = std::static_pointer_cast<BaseComponent>(ptr);

        return ptr;
    }

    // Remove the specified component from the specified entity
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

    // Return pointers to all of the specified components for the specified entity.
    // Those components must exist.
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

    // Return a pointer to the specfied component or a null pointer if that component does not exist
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


    // Return all entities which contain (at least) the specified components.
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

    // Find all entities which have the specified components, and find and return pointers to
    // each of those components.
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

    // Return whether or not the specified entity has the specified components
    template <typename...CT>
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

    // Return the first item in the given list which has the specified component(s) or EntityNull if none of them do
    template <typename...CT>
    EntityRef firstEntityWith( std::vector<EntityRef> const& ents )
    {
        for (auto ent : ents)
        {
            if ( entityHas<CT...>(ent) )
            {
                return ent;
            }
        }

        return EntityNull;
    }

    // Return a const-reference
    std::vector<EntityRef> const& allEntities() const;


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
    std::vector<EntityRef> m_allEnts;

    std::vector<SystemPtr> m_systems;
    std::unordered_map<ComponentId, EntityCompMap> m_components;

    EntityFactory m_entFactory;
};