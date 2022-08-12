#pragma once

#include <game/ActorModifier.h>
#include <game/Combat.h>


namespace ActorMods
{


struct ModAttackRollStatic : public ActorDynamicModImpl<ActorCalc::AttackRoll>
{
    ModAttackRollStatic( int modifier );
    void modify( ActorCalc::AttackRoll* roll ) override;

    int modifier;
};

struct ModSavingThrowStatic : public ActorDynamicModImpl<ActorCalc::SavingThrowRoll>
{
    ModSavingThrowStatic( SavingThrowType type, int modifier );
    void modify( ActorCalc::SavingThrowRoll* roll ) override;

    SavingThrowType type;
    int modifier;
};

struct ModSavingThrowStaticAll : public ActorDynamicModImpl<ActorCalc::SavingThrowRoll>
{
    ModSavingThrowStaticAll( int modifier );
    void modify( ActorCalc::SavingThrowRoll* roll ) override;

    int modifier;
};


struct ModAbilityScoreStatic : public ActorDynamicModImpl<ActorCalc::AbilityScoreBonus>
{
    ModAbilityScoreStatic( AbilityScoreType type, int modifier );
    void modify( ActorCalc::AbilityScoreBonus* roll ) override;

    AbilityScoreType type;
    int modifier;
};

struct ModAbilityScoreStaticAll : public ActorDynamicModImpl<ActorCalc::AbilityScoreBonus>
{
    ModAbilityScoreStaticAll( int modifier );
    void modify( ActorCalc::AbilityScoreBonus* roll ) override;

    int modifier;
};

struct ModMovementSpeedMultiplier : public ActorDynamicModImpl<ActorCalc::MovementSpeedData>
{
    ModMovementSpeedMultiplier( float multiplier );
    void modify( ActorCalc::MovementSpeedData* data ) override;
    
    float multiplier;
};

struct ModACDodgeBonus : public ActorDynamicModImpl<ActorCalc::ArmourClassData>
{
    ModACDodgeBonus( int bonus );
    void modify( ActorCalc::ArmourClassData* data ) override;

    int bonus;
};


}