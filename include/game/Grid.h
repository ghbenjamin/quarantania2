#pragma once

#include <utils/Containers.h>
#include <game/Entity.h>
#include <game/GridFeature.h>
#include <game/Rules.h>

class Grid
{
public:
    explicit Grid( Vector2i bounds );
    ~Grid() = default;

    Grid( const Grid& ) = delete;
    Grid& operator=( const Grid& ) = delete;

    GridFeature<Rules::Passibility, EntityRef>& pass();
    GridFeature<Rules::Visibility, EntityRef>& fov();
    GridFeature<Rules::LightLevel, EntityRef>& light();


private:

    void toBounds( GridRegion* region );
    bool inBounds( Vector2i pos );

    void calculateFOV( Vector2i source, int maxLength );
    void calculateFOVOctant( Vector2i source, int maxLength, int startColumn,
        float leftViewSlope, float rightViewSlope, int octant );


private:
    Vector2i m_bounds;

    GridFeature<Rules::Passibility, EntityRef> m_passGrid;
    GridFeature<Rules::LightLevel, EntityRef> m_lightGrid;
    GridFeature<Rules::Visibility, EntityRef> m_visGrid;

};