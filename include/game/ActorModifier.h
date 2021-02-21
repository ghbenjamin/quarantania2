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
    AbilityScores,
    MovementSpeed,
};

// Union type of the kinds of game object that can be modified
using ModifiableObject = std::variant<
    SavingThrowRoll*,
    AttackRoll*,
    AbilityScoreBonus*,
    MovementSpeedData*
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
    ActorMod( ActorModType type, std::string const& id, std::shared_ptr<ActorModImplBase> impl );
    ~ActorMod() = default;
    
    ActorModType type;
    std::string id;
    std::shared_ptr<ActorModImplBase> impl;
};


// An aggregation of mods which represents a single concept, e.g. a feat or a status, as well as descriptive
// metadata.
class ActorModGroup
{
public:

    ActorModGroup( std::string const& id, std::string const& name, int expiryRound);
    ~ActorModGroup() = default;

    std::string const& getId() const;
    std::string const& getName() const;
    const int getExpiryRound() const;
    
    std::vector<ActorMod> const& getStatMods() const;
    
    template <typename T, typename... Args>
    void addStatMod( ActorModType type, Args... args )
    {
        static_assert( std::is_base_of_v<ActorModImplBase, T> );
        auto mod = std::make_shared<T>( std::forward<Args>(args)... );
        m_statMods.emplace_back( type, m_id, std::static_pointer_cast<ActorModImplBase>(mod) );
    }

private:

    const std::string m_id;       // The ID of this effect. This will be used to clean up after this effect expires.
    const std::string m_name;     // The (display) name of the feat/spell/whatever that spawned this mod
    const int m_expiryRound;      // Do I expire? If so, when?
    
    std::vector<ActorMod> m_statMods;
};
