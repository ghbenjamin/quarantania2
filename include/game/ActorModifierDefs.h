#pragma once

#include <game/ActorModifier.h>
#include <game/Combat.h>


namespace ActorMods
{


struct ModAttackRollStatic : public ActorDynamicModImpl<AttackRoll>
{
    ModAttackRollStatic( int modifier );
    void modify( AttackRoll* roll ) override;

    int modifier;
};

struct ModSavingThrowStatic : public ActorDynamicModImpl<SavingThrowRoll>
{
    ModSavingThrowStatic( SavingThrowType type, int modifier );
    void modify( SavingThrowRoll* roll ) override;

    SavingThrowType type;
    int modifier;
};

struct ModSavingThrowStaticAll : public ActorDynamicModImpl<SavingThrowRoll>
{
    ModSavingThrowStaticAll( int modifier );
    void modify( SavingThrowRoll* roll ) override;

    int modifier;
};


struct ModAbilityScoreStatic : public ActorDynamicModImpl<AbilityScoreBonus>
{
    ModAbilityScoreStatic( AbilityScoreType type, int modifier );
    void modify( AbilityScoreBonus* roll ) override;

    AbilityScoreType type;
    int modifier;
};

struct ModAbilityScoreStaticAll : public ActorDynamicModImpl<AbilityScoreBonus>
{
    ModAbilityScoreStaticAll( int modifier );
    void modify( AbilityScoreBonus* roll ) override;

    int modifier;
};

struct ModMovementSpeedMultiplier : public ActorDynamicModImpl<MovementSpeedData>
{
    ModMovementSpeedMultiplier( float multiplier );
    void modify( MovementSpeedData* data ) override;
    
    float multiplier;
};

struct ModACDodgeBonus : public ActorDynamicModImpl<ArmourClassData>
{
    ModACDodgeBonus( int bonus );
    void modify( ArmourClassData* data ) override;

    int bonus;
};


}