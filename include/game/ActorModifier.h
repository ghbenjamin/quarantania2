#pragma once

#include <string>
#include <memory>
#include <vector>
#include <variant>

#include <game/Combat.h>

class Level;

// The type of calculation which can be modified by affects and abilities
enum class ActorModType
{
    AttackRolls,
    SavingThrows,
};

// Union type of the kinds of game object that can be modified
using ModifiableObject = std::variant<
    SavingThrowRoll*,
    AttackRoll*
        >;


// A function which can modify exactly one of the calculation types enumerated above

struct ActorModImplBase
{};

template <typename T>
struct ActorModImpl : ActorModImplBase
{
    virtual void modify( T* roll ) = 0;
};


// A calculation type + calculation modifier function pair
struct ActorMod
{
    ActorMod( ActorModType type, std::shared_ptr<ActorModImplBase> impl );
    ~ActorMod() = default;
    
    ActorModType type;
    std::shared_ptr<ActorModImplBase> impl;
};


// An aggregation of mods which represents a single concept, e.g. a feat or a status, as well as descriptive
// metadata.
class ActorModGroup
{
public:

    ActorModGroup(std::string const& name, int expiryRound);
    ~ActorModGroup() = default;

    std::string const& getName() const;
    const int getExpiryRound() const;
    
    std::vector<ActorMod> const& getMods() const;
    
    template <typename T, typename... Args>
    void addMod( ActorModType type, Args... args )
    {
        static_assert( std::is_base_of_v<ActorModImplBase, T> );
        auto mod = std::make_shared<T>( std::forward<Args>(args)... );
        m_mods.emplace_back( type, std::static_pointer_cast<ActorModImplBase>(mod) );
    }

private:

    const std::string m_name;     // The name of the feat/spell/whatever that spawned this mod
    const int m_expiryRound;      // Do I expire? If so, when?
    std::vector<ActorMod> m_mods;
};
