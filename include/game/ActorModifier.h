#pragma once

#include <string>
#include <memory>
#include <vector>

class Level;



// A single part of a modifier, for example a conditional buff to a specific attribute.
class ActorModFacet
{

};


using ActorModFacetList = std::vector<std::shared_ptr<ActorModFacet>>;


// An aggregation of mods which represents a single concept, e.g. a feat or a status.
class ActorMod
{
public:

    ActorMod( std::string const& name, int expiryRound, ActorModFacetList& facets );
    ~ActorMod() = default;
    

    std::string const& getName() const;
    const int getExpiryRound() const;

private:

    const std::string m_name;     // The name of the feat/spell/whatever that spawned this mod
    const int m_expiryRound;      // Do I expire? If so, when?
    
    ActorModFacetList m_facets;
};

struct ActorModCompare
{
    bool operator()(ActorMod const& lhs, ActorMod const& rhs) const;
};

