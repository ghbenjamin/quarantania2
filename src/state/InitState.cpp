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

    PlayerGenData pgd;
    pgd.name = "Angus MacKenzie";
    pgd.alignment = Alignment::TN;
    pgd.race = "Human";
    pgd.clazz = "Fighter";

    pgd.attrStr = 16;
    pgd.attrDex = 12;
    pgd.attrCon = 14;
    pgd.attrInt = 9;
    pgd.attrWis = 10;
    pgd.attrCha = 12;

    auto config = generateNextLevelConfig();
    setNextState<LevelState>( config, m_levelCtx, pgd );
}

LevelConfig InitState::generateNextLevelConfig()
{
    LevelConfig config;
    config.size = {41, 25};
    config.roomDensity = 400;

    return config;
}

