#pragma once

#include <game/ActorModifier.h>
#include <game/Combat.h>


namespace ActorMods
{


struct ModAttackRollStatic : public ActorStatModImpl<AttackRoll>
{
    ModAttackRollStatic( int modifier );
    ~ModAttackRollStatic() = default;

    void modify( AttackRoll* roll ) override;

    int modifier;
};

struct ModSavingThrowStatic : public ActorStatModImpl<SavingThrowRoll>
{
    ModSavingThrowStatic( SavingThrowType type, int modifier );
    ~ModSavingThrowStatic() = default;
    void modify( SavingThrowRoll* roll ) override;

    SavingThrowType type;
    int modifier;
};

struct ModSavingThrowStaticAll : public ActorStatModImpl<SavingThrowRoll>
{
    ModSavingThrowStaticAll( int modifier );
    ~ModSavingThrowStaticAll() = default;
    void modify( SavingThrowRoll* roll ) override;

    int modifier;
};


struct ModAbilityScoreStatic : public ActorStatModImpl<AbilityScoreBonus>
{
    ModAbilityScoreStatic( AbilityScoreType type, int modifier );
    ~ModAbilityScoreStatic() = default;
    void modify( AbilityScoreBonus* roll ) override;

    AbilityScoreType type;
    int modifier;
};

struct ModAbilityScoreStaticAll : public ActorStatModImpl<AbilityScoreBonus>
{
    ModAbilityScoreStaticAll( int modifier );
    ~ModAbilityScoreStaticAll() = default;
    void modify( AbilityScoreBonus* roll ) override;

    int modifier;
};

struct ModMovementSpeedMultiplier : public ActorStatModImpl<MovementSpeedData>
{
    ModMovementSpeedMultiplier( float multiplier );
    ~ModMovementSpeedMultiplier() = default;
    void modify( MovementSpeedData* data ) override;
    
    float multiplier;
};


}