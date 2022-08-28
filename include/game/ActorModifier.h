#pragma once

#include <string>
#include <memory>
#include <vector>
#include <variant>
#include <span>

#include <game/Combat.h>
#include <game/Action.h>
#include <utils/Memory.h>

class Actor;
class Weapon;
class Level;


// Mod Components - the basic numerical processes by which modifiers can change specific attributes and rolls
// ----------------------------------

enum class ActorCalcOperation
{
    Add,        // A fixed additive (or subtractive) bonus to a roll
    Multiply,   // A fixed multiplicative bonus to a roll (e.g. halving or doubling)
    Limit,      // Limit the final value to this value
    Floor       // Raise the final value to this floor value if it would be below it
};

// A single modification - a value and an operation (e.g. +5, x2)
struct ActorCalcItem
{
    ActorCalcItem( ActorCalcOperation type, double value );
    
    ActorCalcOperation type {};
    double value = 0;
};


// A list of modifications which are to be combined to a single final result
class ActorCalcList
{
public:
    void addItem( ActorCalcOperation type, double value );
    void addItem( ActorCalcOperation type, int value );

    // Calculate the final effect of the modifiers, for the given base value
    int calculate( int baseValue ) const;
    
    std::span<const ActorCalcItem> items() const;

private:
    std::vector<ActorCalcItem> m_modList;
};



// Dynamic Mods
// ---------------------------------

// The type of calculation which can be modified by affects and abilities
enum class ActorCalculationType
{
    AttackRoll,
    DamageRoll,
    SavingThrow,
    AbilityScore,
    MovementSpeed,
    ArmourClass,
    ActionSpeed,
    CritRange
};



namespace ActorCalc
{
    // A single attack roll to hit, made vs a single attacker.
    struct AttackRoll
    {
        Actor* defender = nullptr;
        Weapon const* weapon = nullptr;
    };
    
    struct DamageRoll
    {
        Actor* defender = nullptr;
    };

    // A bonus to an Ability Score, e.g. STR or DEX. Always given as a raw number, not as a +- modifier
    struct AbilityScoreBonus
    {
        AbilityScoreType type {};
    };
    
    // A bonus to a saving throw, e.g. REF or WILL.
    struct SavingThrowRoll
    {
        SavingThrowType type {};
        EntityRef source = EntityNull;
    };
    
    struct MovementSpeed
    {
    };
    
    struct ArmourClass
    {
    };

    struct ActionSpeed
    {
        GameAction const* action = nullptr;
    };

    struct CritRange
    {
        Actor* defender = nullptr;
        Weapon const* weapon = nullptr;
    };
}


// Union type of the kinds of game stat that can be modified by effects and abilities
using ActorCalcObject = std::variant<
        ActorCalc::SavingThrowRoll,
        ActorCalc::AttackRoll,
        ActorCalc::DamageRoll,
        ActorCalc::AbilityScoreBonus,
        ActorCalc::MovementSpeed,
        ActorCalc::ArmourClass,
        ActorCalc::ActionSpeed,
        ActorCalc::CritRange
    >;

struct ActorCalcData
{
    const Actor* actor;
    
    ActorCalculationType type;
    ActorCalcObject data;
    
    ActorCalcList mods;
};


struct ActorDynamicModImpl
{
    virtual void modify( ActorCalcData& data ) = 0;
};


// A modifier which changes a thing which is happening dynamically like an attack roll or a saving throw (possibly conditionally applied)
// As opposed to a static attack or saving throw modifier which always applied
struct ActorDynamicMod
{
    ActorDynamicMod( ActorCalculationType type, std::string const& id, std::shared_ptr<ActorDynamicModImpl> impl );
    ~ActorDynamicMod() = default;
    
    ActorCalculationType type;
    std::string id;
    std::shared_ptr<ActorDynamicModImpl> impl;
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
    int getExpiryRound() const;
    
    std::vector<ActorDynamicMod> const& getDynamicMods() const;
    std::vector<ActorActionMod> const& getActionMods() const;
    
    template <typename T, typename... Args>
    void addDynamicMod( ActorCalculationType type, Args&&... args )
    {
        m_dynamicMods.emplace_back(type, m_id, utils::make_shared_with_type<ActorDynamicModImpl, T>(
            std::forward<Args>(args)...) );
    }
    
    void addActionMod( GameAction const& action )
    {
        m_actionMods.emplace_back( m_id, action );
    }

private:
    std::string m_id;       // The ID of this effect. This will be used to clean up after this effect expires.
    std::string m_name;     // The (display) name of the feat/spell/whatever that spawned this mod
    int m_expiryRound;      // Do I expire? If so, when?
    
    std::vector<ActorDynamicMod> m_dynamicMods;
    std::vector<ActorActionMod> m_actionMods;
};
