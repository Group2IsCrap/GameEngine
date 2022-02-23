#pragma once

#include "GameEntity.h"
#include "../Components/RenderingComponents.h"

namespace Firelight::ECS
{
    class SpriteEntity : public GameEntity
    {
    public:
        SpriteEntity();

        SpriteEntity(Firelight::ECS::EntityID entityID) : GameEntity(entityID)
        {

        }

        SpriteComponent* GetSpriteComponent();
    };
}
