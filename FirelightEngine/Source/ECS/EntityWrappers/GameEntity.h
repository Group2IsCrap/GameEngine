#pragma once

#include "Entity.h"
#include "../Components/BasicComponents.h"

namespace Firelight::ECS
{
    class GameEntity : public Entity
    {
    public:
        GameEntity();
        GameEntity(std::string name);
        GameEntity(Firelight::ECS::EntityID entityID);
        GameEntity(bool isTemplate, Firelight::ECS::EntityID entityID);

        IdentificationComponent* GetIDComponent();
        StaticComponent*         GetStaticComponent();
        LayerComponent*          GetLayerComponent();
        TransformComponent*      GetTransformComponent();
    };
}
