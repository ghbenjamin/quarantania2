#include <state/InitState.h>
#include <state/LevelState.h>

InitState::InitState()
: m_levelCtx( std::make_shared<LevelContext>() )
{
}

bool InitState::input(IEvent &evt)
{
    return false;
}

void InitState::update(uint32_t ticks, InputInterface &iinter, RenderInterface &rInter)
{
    m_levelCtx->depth++;

    PCData p1;
    p1.name = "Angus MacKenzie";
    p1.alignment = DnDAlignment::TN;
    p1.race = "Human";
    p1.clazz = "Fighter";
    p1.sprite = { "dawnlike_chars", "Player_001" };

    p1.attrStr = 16;
    p1.attrDex = 12;
    p1.attrCon = 14;
    p1.attrInt = 9;
    p1.attrWis = 10;
    p1.attrCha = 12;

    PCData p2;
    p2.name = "Gwendlyn Di Corci";
    p2.alignment = DnDAlignment::TN;
    p2.race = "Human";
    p2.clazz = "Fighter";
    p2.sprite = { "dawnlike_chars", "Player_003" };

    p2.attrStr = 16;
    p2.attrDex = 12;
    p2.attrCon = 14;
    p2.attrInt = 9;
    p2.attrWis = 10;
    p2.attrCha = 12;

    PartyData pdata;
    pdata.playerChars = { p1, p2 };

    auto config = generateNextLevelConfig();
    setNextState<LevelState>( config, m_levelCtx, pdata );
}

RandomLevelConfig InitState::generateNextLevelConfig()
{
    RandomLevelConfig config;
    config.size = {21, 17};
    config.roomDensity = 400;

    return config;
}

