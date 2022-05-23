#include "CombatCalculations.h"
#include <Source/ECS/EntityWrappers/Entity.h>
#include "../Core/CharacterEntity.h"
#include "../Core/AIEntity.h"
#include "../Core/ResourceEntity.h"

void CombatCalculations::PlaceSphere(Facing dir, Vec3f point)
{
    /*float directionalAngle = 0.0f;
    float weaponAngle;*/
    //add to function call

    Vec3f offset = Vec3f(0.0f,0.0f,0.0f);
    float radius = 0;

    switch (dir)
    {
    case Facing::Up:
        offset = Vec3f(0.0f, 1.0f, 0.0f);
        radius = 1.5f;
        break;
    case Facing::Down:
        offset = Vec3f(0.0f, -1.0f, 0.0f);
        radius = 1.5f;
        break;
    case Facing::Left:
        offset = Vec3f(-1.5f, 0.0f, 0.0f);
        radius = 2.0f;
        break;
    case Facing::Right:
        offset = Vec3f(1.5f, 0.0f, 0.0f);
        radius = 2.0f;
        break;
    }

    std::vector<int> layers = {static_cast<int>(GameLayer::Enemy), static_cast<int>(GameLayer::Resource)};
    

    for (int i = 0; i < layers.size(); i++)
    {
        std::vector<Firelight::ECS::Entity*> targets = PhysicsHelpers::OverlapCircle(point + offset, radius, layers[i]);
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