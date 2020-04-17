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

    pgd.attrStr = 10;
    pgd.attrDex = 10;
    pgd.attrCon = 10;
    pgd.attrInt = 10;
    pgd.attrWis = 10;
    pgd.attrCha = 10;

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

