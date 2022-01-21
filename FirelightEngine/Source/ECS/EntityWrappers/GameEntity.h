#pragma once

#include "../Entity.h"
#include "../Components/BasicComponents.h"

namespace Firelight::ECS
{
    class GameEntity : public Entity
    {
    public:
        GameEntity();

        IdentificationComponent* GetIDComponent();
        StaticComponent*         GetStaticComponent();
        TransformComponent*      GetTransformComponent();
    };
}
