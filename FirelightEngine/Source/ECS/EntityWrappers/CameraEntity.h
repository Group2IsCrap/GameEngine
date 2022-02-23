#pragma once

#include "GameEntity.h"
#include "../Components/RenderingComponents.h"

namespace Firelight::ECS
{
	class CameraEntity : public GameEntity
    {
    public:
        CameraEntity();

        Camera2DComponent* GetCamera2DComponent();
    };
}
