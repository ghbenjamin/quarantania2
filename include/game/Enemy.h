#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include <resource/Spritesheet.h>
#include <game/DiceRoll.h>

// Forward declarations
class Enemy;
class EnemyBehaviour;
class EnemyManager;
struct EnemyData;

// Typedefs
using EnemyBehaviourPtr = std::unique_ptr<EnemyBehaviour>;


class Enemy
{
    friend class EnemyManager;

public:
    ~Enemy() = default;

    SpritesheetKey const& sprite() const;
    EnemyData const& data() const;

private:
    Enemy(EnemyData const& data);

    EnemyData const& m_data;
    EnemyBehaviourPtr m_behaviour;
};

struct EnemyData
{
    // Meta
    std::string name;
    SpritesheetKey sprite;
    int xp;

    // Defence
    int maxHP;
    int ac;
    int fortSave;
    int refSave;
    int willSave;

    // Stats
    int strScore;
    int dexScore;
    int conScore;
    int intScore;
    int wisScore;
    int chaScore;

    // Misc
    int speed;

    // Attack
    DiceRoll attack;
};

class EnemyBehaviour
{
public:
private:
};


class EnemyManager
{
public:
    EnemyManager() = default;
    ~EnemyManager() = default;

    void loadAllData();

    Enemy createEnemy( std::string const& name ) const;

private:
    std::unordered_map<std::string, const EnemyData> m_enemyData;
};