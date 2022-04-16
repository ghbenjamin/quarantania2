#pragma once

#include <string>
#include <memory>
#include <vector>
#include <variant>

#include <game/Combat.h>
#include <game/Action.h>
#include <utils/Memory.h>

class Level;

// The type of calculation which can be modified by affects and abilities
enum class ActorDynamicModType
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

enum class ActorStaticModType
{
    AttrStr,
    AttrDex,
    AttrCon,
    AttrInt,
    AttrWis,
    AttrCha,
    SaveFort,
    SaveRef,
    SaveWill
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
struct ActorStatDynamicImplBase {};

template <typename T>
struct ActorDynamicModImpl : ActorStatDynamicImplBase
{
    virtual void modify( T* roll ) = 0;
};


// A modifier which applies a fixed static modifier to a single stat, like an attribute or saving throw buff
struct ActorStaticMod
{
    ActorStaticMod(std::string const &id, ActorStaticModType type, int value);
    
    std::string id;
    ActorStaticModType type;
    int value;
};


// A modifier which changes a thing which is happening dynamically like an attack roll or a saving throw (possibly conditionally applied)
// As opposed to a static attack or saving throw modifier which always applied
struct ActorDynamicMod
{
    ActorDynamicMod( ActorDynamicModType type, std::string const& id, std::shared_ptr<ActorStatDynamicImplBase> impl );
    ~ActorDynamicMod() = default;
    
    ActorDynamicModType type;
    std::string id;
    std::shared_ptr<ActorStatDynamicImplBase> impl;
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
    
    std::vector<ActorDynamicMod> const& getDynamicMods() const;
    std::vector<ActorActionMod> const& getActionMods() const;
    std::vector<ActorStaticMod> const& getStaticMods() const;
    
    template <typename T, typename... Args>
    void addDynamicMod( ActorDynamicModType type, Args&&... args )
    {
        m_dynamicMods.emplace_back(type, m_id,
                                   utils::make_shared_with_type<ActorStatDynamicImplBase, T>( std::forward<Args>(args)...) );
    }
    
    void addActionMod( GameAction const& action )
    {
        m_actionMods.emplace_back( m_id, action );
    }
    
    void addStaticMod( ActorStaticModType type, int value )
    {
        m_staticMods.emplace_back( m_id, type, value );
    }

private:
    std::string m_id;       // The ID of this effect. This will be used to clean up after this effect expires.
    std::string m_name;     // The (display) name of the feat/spell/whatever that spawned this mod
    int m_expiryRound;      // Do I expire? If so, when?
    
    std::vector<ActorDynamicMod> m_dynamicMods;
    std::vector<ActorActionMod> m_actionMods;
    std::vector<ActorStaticMod> m_staticMods;
};
