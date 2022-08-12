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
// ----------------------------------

enum class ModComponentType
{
    Add,        // A fixed additive (or subtractive) bonus to a roll
    Multiply,   // A fixed multiplicative bonus to a roll (e.g. halving or doubling)
    Limit,      // Limit the final value to this value
    Floor       // Raise the final value to this floor value if it would be below it
};

struct ModComponent
{
    ModComponentType type {};
    double value = 0;
};

class ModComponentList
{
public:

    void addModComponent(ModComponentType type, double value);
    void addModComponent(ModComponentType type, int value);

    int calculate() const;

private:
    std::vector<ModComponent> m_modList; 
};



// Dynamic Mods
// ---------------------------------

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
};

struct ActorModifiableData
{
public:
    
    ActorModifiableData( Actor* target, int baseValue );
    ActorModifiableData( Actor* target, int baseValue, EntityRef source );
    virtual ~ActorModifiableData() = default;
    
    int baseValue() const;
    EntityRef target() const;
    EntityRef source() const;
    Actor* actor() const;
    ModComponentList& modList();
    
protected:
    int m_baseValue;
    Actor* m_actor;
    EntityRef m_target;
    EntityRef m_other;
    ModComponentList m_modList;
};


// A bonus to an Ability Score, e.g. STR or DEX. Always given as a raw number, not as a +- modifier
struct AbilityScoreBonus : public ActorModifiableData
{
    using ActorModifiableData::ActorModifiableData;
    AbilityScoreType type {};
};

// A bonus to a saving throw, e.g. REF or WILL.
struct SavingThrowRoll : public ActorModifiableData
{
    using ActorModifiableData::ActorModifiableData;
    SavingThrowType type {};
};

// A single attack roll to hit, made vs a single attacker.
struct AttackRoll : public ActorModifiableData
{
    using ActorModifiableData::ActorModifiableData;

    Actor* defender = nullptr;
    Weapon const* weapon = nullptr;
    int targetValue = -1;
    bool isHit = false;
    bool isCrit = false;
};

struct DamageRoll
{
    int naturalRoll = -1;
    int modifiedRoll = -1;
};

struct MovementSpeedData : public ActorModifiableData
{
    using ActorModifiableData::ActorModifiableData;
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
    ActionSpeedData*
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
    void addDynamicMod( ActorDynamicModType type, Args&&... args )
    {
        m_dynamicMods.emplace_back(type, m_id,
                                   utils::make_shared_with_type<ActorStatDynamicImplBase, T>( std::forward<Args>(args)...) );
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
