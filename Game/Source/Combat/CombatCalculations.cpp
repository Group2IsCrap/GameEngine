#include "CombatCalculations.h"
#include <Source/ECS/EntityWrappers/Entity.h>
#include "../Core/CharacterEntity.h"
#include "../Core/AIEntity.h"

void CombatCalculations::PlaceSphere(Facing dir, Vec3f nextPosition, bool canAttack)
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
        directionalAngle = -90.0f;
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
        std::vector<Firelight::ECS::Entity*> targets = PhysicsHelpers::OverlapCone(nextPosition, 2.0f, directionalAngle, 2.0f, layers[i]);
        if (targets.empty())
        {
            continue;
        }
        else
        {
            for (auto* target : targets)
            {
                AIEntity* currentEntity = new AIEntity(target->GetEntityID());
                currentEntity->RemoveHealth(1);
            }
            break;
        }
    }
}