#include "CombatCalculations.h"
#include <Source/ECS/EntityWrappers/Entity.h>
#include "../Core/CharacterEntity.h"
#include "../Core/AIEntity.h"
#include "../Core/ResourceEntity.h"

void CombatCalculations::PlaceSphere(Facing dir, Vec3f point)
{
    float directionalAngle = 0.0f;
    float weaponAngle;
    //add to function call

    switch (dir)
    {
    case Facing::Up:
        directionalAngle = 90.0f;
        break;
    case Facing::Down:
        directionalAngle = 270.0f;
        break;
    case Facing::Left:
        directionalAngle = 180.0f;
        break;
    case Facing::Right:
        directionalAngle = 0.0f;
        break;
    }

    std::vector<int> layers = {static_cast<int>(GameLayer::Enemy), static_cast<int>(GameLayer::Resource)};
    

    for (int i = 0; i < layers.size(); i++)
    {
        std::vector<Firelight::ECS::Entity*> targets = PhysicsHelpers::OverlapCone(point, 10.0f, directionalAngle, 10.0f, layers[i]);
        if (targets.empty())
        {
            continue;
        }
        else
        {
            for (auto* target : targets)
            {
                if (target->GetComponent<LayerComponent>()->layer == static_cast<int>(GameLayer::Resource))
                {
                    ResourceEntity* resourceEntity = new ResourceEntity(target->GetEntityID());
                    resourceEntity->RemoveHealth(1);
                }
                else if (target->GetComponent<LayerComponent>()->layer == static_cast<int>(GameLayer::Enemy))
                {
                    AIEntity* aiEntity = new AIEntity(target->GetEntityID());
                    aiEntity->RemoveHealth(1);
                }
            }
            break;
        }
    }
}