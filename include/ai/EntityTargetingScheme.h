#pragma once

#include <optional>
#include <engine/Entity.h>

class Level;

class EntityTargetingScheme
{
public:
    virtual std::optional<EntityRef> getEntity(Level* level, EntityRef actor) = 0;
};


class EntityTargetingNearest : public EntityTargetingScheme
{
public:
    std::optional<EntityRef> getEntity(Level* level, EntityRef actor) override;
};
