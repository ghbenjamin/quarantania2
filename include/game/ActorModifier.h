#pragma once

#include <string>
#include <memory>
#include <vector>
#include <variant>

#include <game/Combat.h>
#include <game/Action.h>
#include <utils/Memory.h>

class Actor;
class Weapon;
class Level;


// Mod Components - the basic numerical processes by which modifiers can change specific attributes and rolls

enum class ModComponentType
{
    Add,        // A fixed additive (or subtractive) bonus to a roll
    Multiply,   // A fixed multiplicative bonus to a roll (e.g. halving or doubling)
    Limit,      // Limit the final value to this value
    Floor       // Raise the final value to this floor value if it would be below it
};

struct ModComponent
{
    ModComponentType type;
    double value;
};


// Dynamic Mods

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

// Data which will be used by all rolls & opposed rolls
struct ActorRollData
{
    // Either source or target may be null
    EntityRef source = EntityNull;   // Where did this action originate?
    EntityRef target = EntityNull;   // What is the target of this action?

    int naturalRoll = -1;    // What was the actual number rolled?
    int finalRoll = -1;      // The final value after all modifiers have been considered

    std::vector<ModComponent> modComponents;
};


// A bonus to an Ability Score, e.g. STR or DEX. Always given as a raw number, not as a +- modifier
struct AbilityScoreBonus
{
    ActorRollData ctx;
    AbilityScoreType type = AbilityScoreType::CHA;
};

// A bonus to a saving throw, e.g. REF or WILL.
struct SavingThrowRoll
{
    ActorRollData ctx;
    SavingThrowType type {};
};

// A single instance of a melee attack, e.g. one half of a double attack.
struct SingleMeleeAttackInstance
{
    ActorRollData ctx;
    Actor* attacker;
    Actor* defender;
    Weapon const* weapon;
};

// A single weapon - attack mod pair to help describe how many attacks an attacker gets for a given attack type
struct MeleeAttackCountItem
{
    Weapon const* weapon = nullptr;
    int naturalAttackMod = -1;
};

struct MeleeAttackCountData
{
    std::vector<MeleeAttackCountItem> attacks;
};

// A single attack roll to hit, made vs a single attacker.
struct AttackRoll
{
    ActorRollData ctx;

    int targetValue = -1;
    bool isHit = false;
    bool isCrit = false;
};

struct DamageRoll
{
    int naturalRoll = -1;
    int modifiedRoll = -1;
};

struct MovementSpeedData
{
    ActorRollData ctx;

    int moveSpeedLimit = -1;
    float multiplier = 1.0f;
};

struct ArmourClassData
{
    int shieldBonus = 0;
    int armourBonus = 0;

    int dexBonus = 0;
    std::optional<int> maxDexToAc = {};

    int dodgeBonus = 0;
    int staticBonus = 10;

    int modifiedAC = 0;
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


// Static mods
// ----------------------------

// The attributes of an actor that we allow to be statically modified
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

// A modifier which applies a fixed static modifier to a single stat, like an attribute or saving throw buff
struct ActorStaticMod
{
    ActorStaticMod(std::string const &id, ActorStaticModType type, int value);

    std::string id;
    ActorStaticModType type;
    int value;
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
