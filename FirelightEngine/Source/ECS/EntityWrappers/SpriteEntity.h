#pragma once

#include "GameEntity.h"
#include "../Components/RenderingComponents.h"

namespace Firelight::ECS
{
    class SpriteEntity : public GameEntity
    {
    public:
        SpriteEntity();

        SpriteComponent* GetSpriteComponent();
    };
}
