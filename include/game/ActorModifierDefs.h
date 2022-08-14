#pragma once

#include <game/ActorModifier.h>
#include <game/Combat.h>


namespace ActorMods
{


struct ModAttackRollStatic : public ActorDynamicModImpl
{
    ModAttackRollStatic( int modifier );
    void modify( ActorCalcData& data ) override;
    
    int modifier;
};

struct ModSavingThrowStatic : public ActorDynamicModImpl
{
    ModSavingThrowStatic( SavingThrowType type, int modifier );
    void modify( ActorCalcData& data ) override;
    
    SavingThrowType type;
    int modifier;
};

struct ModSavingThrowStaticAll : public ActorDynamicModImpl
{
    ModSavingThrowStaticAll( int modifier );
    void modify( ActorCalcData& data ) override;
    
    int modifier;
};


struct ModAbilityScoreStatic : public ActorDynamicModImpl
{
    ModAbilityScoreStatic( AbilityScoreType type, int modifier );
    void modify( ActorCalcData& data ) override;
    
    AbilityScoreType type;
    int modifier;
};

struct ModAbilityScoreStaticAll : public ActorDynamicModImpl
{
    ModAbilityScoreStaticAll( int modifier );
    void modify( ActorCalcData& data ) override;
    
    int modifier;
};

struct ModMovementSpeedMultiplier : public ActorDynamicModImpl
{
    ModMovementSpeedMultiplier( float multiplier );
    void modify( ActorCalcData& data ) override;
    
    float multiplier;
};



}