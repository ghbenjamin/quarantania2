#pragma once

#include <game/ActorModifier.h>


//  Facet Definitions
// ------------------------


namespace ActorModFacets
{





}


//  Modifier Definitions
// ------------------------


struct ActorModStatusSickened : public ActorModImpl
{
    ActorModFacetList addFacets() override;
};
