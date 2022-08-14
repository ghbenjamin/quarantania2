#include <game/ActorModifierDefs.h>




ActorMods::ModSavingThrowStatic::ModSavingThrowStatic( SavingThrowType type, int modifier )
        : type(type), modifier(modifier) {}

void ActorMods::ModSavingThrowStatic::modify( ActorCalcData& data )
{
    auto& ctx = std::get<ActorCalc::SavingThrowRoll>( data.data );
    if (ctx.type == type)
    {
        data.mods.addItem(ActorCalcOperation::Add, modifier);
    }
}


ActorMods::ModAttackRollStatic::ModAttackRollStatic( int modifier )
        : modifier(modifier) {}

void ActorMods::ModAttackRollStatic::modify( ActorCalcData& data )
{
    data.mods.addItem(ActorCalcOperation::Add, modifier);
}


ActorMods::ModSavingThrowStaticAll::ModSavingThrowStaticAll( int modifier )
        : modifier(modifier) {}

void ActorMods::ModSavingThrowStaticAll::modify( ActorCalcData& data )
{
    data.mods.addItem(ActorCalcOperation::Add, modifier);
}

ActorMods::ModAbilityScoreStatic::ModAbilityScoreStatic(AbilityScoreType type, int modifier)
        : type(type), modifier(modifier) {}

void ActorMods::ModAbilityScoreStatic::modify( ActorCalcData& data )
{
    auto& ctx = std::get<ActorCalc::AbilityScoreBonus>( data.data );

    if ( ctx.type == type )
    {
        data.mods.addItem(ActorCalcOperation::Add, modifier);
    }
}

ActorMods::ModAbilityScoreStaticAll::ModAbilityScoreStaticAll(int modifier)
        : modifier(modifier) {}

void ActorMods::ModAbilityScoreStaticAll::modify( ActorCalcData& data )
{
    data.mods.addItem(ActorCalcOperation::Add, modifier);
}


ActorMods::ModMovementSpeedMultiplier::ModMovementSpeedMultiplier( float multiplier )
        : multiplier(multiplier) {}

void ActorMods::ModMovementSpeedMultiplier::modify( ActorCalcData& data )
{
    data.mods.addItem(ActorCalcOperation::Multiply, multiplier);
}

