#pragma once

#include <string>
#include <memory>
#include <vector>

class Level;



// A single part of a modifier, for example a conditional buff to a specific attribute.
class ActorModFacet
{

};




using ActorModFacetList = std::vector<std::unique_ptr<ActorModFacet>>;


struct ActorModImpl
{
    virtual ActorModFacetList addFacets() = 0;
};


// An aggregation of mods which represents a single concept, e.g. a feat or a status.
class ActorMod
{
public:

    ActorMod( std::string const& name, int expiryRound, std::unique_ptr<ActorModImpl> impl );
    ~ActorMod() = default;

    std::string const& getName() const;
    const int getExpiryRound() const;

private:

    const std::string m_name;     // The name of the feat/spell/whatever that spawned this mod
    const int m_expiryRound;      // Do I expire? If so, when?

   // std::unique_ptr<ActorModImpl> m_impl;
};

struct ActorModCompare
{
    bool operator()(ActorMod const& lhs, ActorMod const& rhs) const;
};

