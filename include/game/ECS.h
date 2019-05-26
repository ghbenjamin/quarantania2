#pragma once

#include <unordered_map>
#include <algorithm>
#include <type_traits>
#include <typeindex>
#include <set>
#include <iterator>

#include <game/Component.h>
#include <game/Entity.h>
#include <game/System.h>
#include <utils/IdPool.h>
#include <utils/Assert.h>


using EntityCompMap = std::unordered_map<EntityRef, ComponentPtr>;

class ECS
{
public:
    ECS();
    ~ECS() = default;

    virtual void update(uint32_t ticks, RenderInterface &rInter);

    EntityRef createEntity();
    void deleteEntity(EntityRef ent);
    void disableEntity(EntityRef ent);
    void enableEntity(EntityRef ent);

    template <typename CT, typename... Args>
    void addComponent(EntityRef ent, Args...args)
    {
        auto ti = std::type_index(typeid(CT));
        auto ptr = std::make_shared<CT>( std::forward<Args>(args)... );
        mapForComponent<CT>()[ent] = std::static_pointer_cast<Component>(ptr);
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

    template <typename CT>
    std::shared_ptr<CT> const& getComponent(EntityRef ent)
    {
        auto bPtr = mapForComponent<CT>()[ent];
        Assert( !!bPtr, "Entity does not have " );
        auto dPtr = std::static_pointer_cast<CT>(bPtr);
        return std::move(dPtr);
    }

    template<typename... Component>
    decltype(auto) get(const EntityRef entity)
    {
        if constexpr(sizeof...(Component) == 1)
        {
            return (std::static_pointer_cast<Component>(mapForComponent<Component>()[entity]), ...);
        }
        else
        {
            return std::tuple<std::shared_ptr<Component>...>{
                std::static_pointer_cast<Component>(get<Component>(entity))...};
        }
    }

    /**
     * Return all Entities which contain (at least) the specified components.
     */
    template<typename... Component>
    std::vector<EntityRef> having()
    {
        std::set<EntityRef> matching;
        std::set<EntityRef> mbuffer;
        std::set<EntityRef> inter;

        auto typeids = { std::type_index(typeid(Component))... };
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
    template<typename... Component>
    decltype(auto) with()
    {
        std::vector<std::tuple<std::shared_ptr<Component>...>> out;

        for ( auto const ent : having<Component...>() )
        {
            out.push_back({ get<Component>(ent) ... });
        }

        return out;
    }

private:

    template <typename CT>
    EntityCompMap& mapForComponent()
    {
        auto ti = std::type_index(typeid(CT));
        auto it = m_components.find(ti);
        return it->second;
    }

    template <typename CT, typename = typename std::enable_if<std::is_base_of<Component, CT>::value>::type>
    void registerComponent()
    {
        auto ti = std::type_index(typeid(CT));
        m_components[ti] = EntityCompMap();
    }

    template <typename ST, typename = typename std::enable_if<std::is_base_of<System, ST>::value>::type>
    void registerSystem()
    {
        auto dPtr = std::make_unique<ST>( this );
        auto bPtr = std::unique_ptr<System>( dPtr.release() );
        m_systems.push_back( std::move(bPtr) );
    }


private:

    IdPool<EntityRef> m_entityPool;
    std::unordered_map<std::type_index, EntityCompMap> m_components;
    std::vector<SystemPtr> m_systems;

};