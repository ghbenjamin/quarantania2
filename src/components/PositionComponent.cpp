#include <components/PositionComponent.h>
#include <utils/GlobalConfig.h>

PositionComponent::PositionComponent(Vector2i const &tilePos, Vector2i const& pxPos)
    : tilePosition(tilePos), pixelPosition(pxPos) {}

PositionComponent::PositionComponent(const Vector2i &p)
    : tilePosition(p), pixelPosition(p * GlobalConfig::TileSizePx) {}