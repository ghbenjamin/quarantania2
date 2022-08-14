#pragma once

struct ActorCalcData;

class MeleeAttack
{
public:
    MeleeAttack() = default;
    virtual ~MeleeAttack() = default;
    
    virtual void modifyAttackRoll( ActorCalcData& data );
    virtual void modifyDamageRoll( ActorCalcData& data );
    virtual void onHit();
    virtual bool isFullAttack();
};

