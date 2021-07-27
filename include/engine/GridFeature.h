#pragma once

#include <vector>
#include <unordered_map>

#include <utils/Assert.h>
#include <utils/Containers.h>
#include <utils/GridUtils.h>

template <typename DataType, typename RefType, typename InternalType = std::uint8_t>
class GridFeature
{

public:
    explicit GridFeature( Vector2i bounds )
        : m_bounds(bounds), m_doCache(true), m_size(bounds.x() * bounds.y()),
          m_fixed( m_size, 0 ),  m_cached( m_size, 0 )
    {
    }

    ~GridFeature() { }

    void setFixed( GridRegion region, DataType dt )
    {
        for ( auto const& v : region )
        {
            setFixed( v, dt );
        }
    }

    void setFixed( Vector2i tile, DataType dt )
    {
        auto idx = vectorToIdx(tile);
        m_fixed[idx] = static_cast<InternalType>(dt);
        recacheTile( idx );
    }

    void setFixed( std::size_t idx, DataType dt )
    {
        m_fixed[idx] = static_cast<InternalType>(dt);
        recacheTile( idx );
    }

    void setDynamic( GridRegion region, RefType rt, DataType dt )
    {
        for ( auto const& t: region )
        {
            setDynamic( t, rt, dt );
        }
    }

    void setDynamic( Vector2i tile, RefType rt, DataType dt )
    {
        auto idx = vectorToIdx(tile);
        m_dynamic.insert( std::make_pair( idx, std::make_pair(static_cast<InternalType>(dt), rt) ) );
        recacheTile( idx );
    }

    void removeDynamic( GridRegion region, RefType rt, DataType dt )
    {
        for ( auto const& t: region )
        {
            removeDynamic( t, rt, dt );
        }
    }
    void removeDynamic( Vector2i tile, RefType rt, DataType dt )
    {
        auto idx = vectorToIdx(tile);

        auto range_it = m_dynamic.equal_range(idx);
        for ( auto it = range_it.first; it != range_it.second; it++ )
        {
            if ( it->second == std::make_pair(static_cast<InternalType>(dt), rt) )
            {
                m_dynamic.erase(it);
                break;
            }
        }

        recacheTile( idx );
    }

    void moveDynamic( Vector2i oldTile, Vector2i newTile, RefType rt, DataType dt )
    {
        setDynamic( newTile, rt, dt );
        removeDynamic( oldTile, rt, dt );
    }

    DataType valueAt( Vector2i pos )
    {
		auto idx = vectorToIdx(pos);
		Assert(idx < m_cached.size());
        return static_cast<DataType>(m_cached[idx]);
    }

    DataType valueAt( int idx )
    {
        return static_cast<DataType>(m_cached[ idx ]);
    }

    void disableCache( )
    {
        m_doCache = false;
    }

    void enableCache()
    {
        m_doCache = true;
        recacheAll();
    }


private:

    void recacheAll()
    {
        if (!m_doCache) return;
        m_cached = m_fixed;

        for ( auto const&[k, v] : m_dynamic )
        {
            recacheTile( k );
        }
    }

    void recacheTile( std::size_t idx )
    {
        if (!m_doCache) return;

        InternalType base = m_fixed.at(idx);
        InternalType dyn = 0;

        auto range_it = m_dynamic.equal_range( idx );
        for ( auto it = range_it.first; it != range_it.second; it++ )
        {
            if ( it->second.first > dyn )
            {
                dyn = it->second.first;
            }
        }

        m_cached[idx] = std::max(base, dyn);
    }

    inline size_t vectorToIdx( Vector2i vec )
    {
        return vec.x() + ( m_bounds.x() * vec.y() );
    }

    Vector2i m_bounds;
    bool m_doCache;
    std::size_t m_size;

    std::vector<InternalType> m_fixed;
    std::vector<InternalType> m_cached;
    std::unordered_multimap<std::size_t, std::pair<InternalType, RefType>> m_dynamic;
};