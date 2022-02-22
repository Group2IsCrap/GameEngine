#pragma once

#include "Entity.h"
#include "../Components/BasicComponents.h"

namespace Firelight::ECS
{
    class GameEntity : public Entity
    {
    public:
        GameEntity();
        GameEntity(Firelight::ECS::EntityID entityID);

        IdentificationComponent* GetIDComponent();
        StaticComponent*         GetStaticComponent();
        TransformComponent*      GetTransformComponent();
    };
}
