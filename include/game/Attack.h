#pragma once

namespace ActorCalc
{
struct AttackRoll;
struct DamageRoll;
}


class MeleeAttack
{
public:
    MeleeAttack() = default;
    virtual ~MeleeAttack() = default;
    
    virtual void modifyAttackRoll( ActorCalc::AttackRoll& roll );
    virtual void modifyDamageRoll( ActorCalc::DamageRoll& roll );
    virtual void onHit();
    virtual bool isFullAttack();
};

