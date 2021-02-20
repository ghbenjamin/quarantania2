#pragma once

#include <game/ActorModifier.h>
#include <game/Combat.h>


namespace ActorMods
{


struct ModAttackRollStatic : public ActorModImpl<AttackRoll>
{
    ModAttackRollStatic( int modifier );
    ~ModAttackRollStatic() = default;

    void modify( AttackRoll* roll ) override;

    int modifier;
};

struct ModSavingThrowStatic : public ActorModImpl<SavingThrowRoll>
{
    ModSavingThrowStatic( SavingThrowType type, int modifier );
    ~ModSavingThrowStatic() = default;
    void modify( SavingThrowRoll* roll ) override;

    SavingThrowType type;
    int modifier;
};

struct ModSavingThrowStaticAll : public ActorModImpl<SavingThrowRoll>
{
    ModSavingThrowStaticAll( int modifier );
    ~ModSavingThrowStaticAll() = default;
    void modify( SavingThrowRoll* roll ) override;

    int modifier;
};


struct ModAbilityScoreStatic : public ActorModImpl<AbilityScoreBonus>
{
    ModAbilityScoreStatic( AbilityScoreType type, int modifier );
    ~ModAbilityScoreStatic() = default;
    void modify( AbilityScoreBonus* roll ) override;

    AbilityScoreType type;
    int modifier;
};

struct ModAbilityScoreStaticAll : public ActorModImpl<AbilityScoreBonus>
{
    ModAbilityScoreStaticAll( int modifier );
    ~ModAbilityScoreStaticAll() = default;
    void modify( AbilityScoreBonus* roll ) override;

    int modifier;
};

}