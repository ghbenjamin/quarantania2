#pragma once

struct AttackRoll;
struct DamageRoll;

class MeleeAttack
{
public:
    MeleeAttack() = default;
    virtual ~MeleeAttack() = default;
    
    virtual void modifyAttackRoll( AttackRoll& roll );
    virtual void modifyDamageRoll( DamageRoll& roll );
    virtual void onHit();
    virtual bool isFullAttack();
};

