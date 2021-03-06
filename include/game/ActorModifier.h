#pragma once

#include <string>
#include <memory>
#include <vector>
#include <variant>

#include <game/Combat.h>
#include <game/Action.h>

class Level;

// The type of calculation which can be modified by affects and abilities
enum class ActorStatModType
{
    AttackRolls,
    DamageRolls,
    SavingThrows,
    AbilityScores,
    MovementSpeed,
    ArmourClassData,
    ActionSpeedData,
    MeleeAttackCountData,
};

// Union type of the kinds of game stat that can be modified by effects and abilities
using ModifiableStatObject = std::variant<
    SavingThrowRoll*,
    AttackRoll*,
    DamageRoll*,
    AbilityScoreBonus*,
    MovementSpeedData*,
    ArmourClassData*,
    ActionSpeedData*,
    MeleeAttackCountData*
        >;


// A function which can modify exactly one of the calculation types enumerated above
struct ActorStatModImplBase {};

template <typename T>
struct ActorStatModImpl : ActorStatModImplBase
{
    virtual void modify( T* roll ) = 0;
};


// A calculation type + calculation modifier function pair
struct ActorStatMod
{
    ActorStatMod(ActorStatModType type, std::string const& id, std::shared_ptr<ActorStatModImplBase> impl );
    ~ActorStatMod() = default;
    
    ActorStatModType type;
    std::string id;
    std::shared_ptr<ActorStatModImplBase> impl;
};

// A new action granted by a modifier
struct ActorActionMod
{
    ActorActionMod( std::string const &id, GameAction const &action );
    
    std::string id;
    GameAction action;
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
    
    std::vector<ActorStatMod> const& getStatMods() const;
    std::vector<ActorActionMod> const& getActionMods() const;
    
    template <typename T, typename... Args>
    void addStatMod( ActorStatModType type, Args... args )
    {
        static_assert( std::is_base_of_v<ActorStatModImplBase, T> );
        auto mod = std::make_shared<T>( std::forward<Args>(args)... );
        m_statMods.emplace_back( type, m_id, std::static_pointer_cast<ActorStatModImplBase>(mod) );
    }
    
    void addActionMod( GameAction const& action )
    {
        m_actionMods.emplace_back( m_id, action );
    }

private:

    const std::string m_id;       // The ID of this effect. This will be used to clean up after this effect expires.
    const std::string m_name;     // The (display) name of the feat/spell/whatever that spawned this mod
    const int m_expiryRound;      // Do I expire? If so, when?
    
    std::vector<ActorStatMod> m_statMods;
    std::vector<ActorActionMod> m_actionMods;
};
