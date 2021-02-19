#pragma once

#include <string>
#include <memory>
#include <vector>

class Level;



// An aggregation of mods which represents a single concept, e.g. a feat or a status.
class ActorModGroup
{
public:

    ActorModGroup(std::string const& name, int expiryRound);
    ~ActorModGroup() = default;

    std::string const& getName() const;
    const int getExpiryRound() const;

private:

    const std::string m_name;     // The name of the feat/spell/whatever that spawned this mod
    const int m_expiryRound;      // Do I expire? If so, when?
};

struct ActorModCompare
{
    bool operator()(ActorModGroup const& lhs, ActorModGroup const& rhs) const;
};

