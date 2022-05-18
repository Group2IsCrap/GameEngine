#include "CombatCalculations.h"
#include <Source/ECS/EntityWrappers/Entity.h>
#include "../Core/CharacterEntity.h"
#include "../Core/AIEntity.h"

void CombatCalculations::PlaceSphere(Facing dir, Vec3f nextPosition, float offset)
{
    Vec3f offsetVector;

    switch (dir)
    {
    case Facing::Up:
        offsetVector = Vec3f(0.0f, offset, 0.0f);
        break;
    case Facing::Down:
        offsetVector = Vec3f(0.0f, -offset, 0.0f);
        break;
    case Facing::Left:
        offsetVector = Vec3f(-offset, 0.0f, 0.0f);
        break;
    case Facing::Right:
        offsetVector = Vec3f(offset, 0.0f, 0.0f);
        break;
    }


    std::vector<Firelight::ECS::Entity*> targets = PhysicsHelpers::OverlapCircle(nextPosition + offsetVector, 1.5, static_cast<int>(GameLayer::Enemy));
    for (auto* target : targets)
    {
        AIEntity* currentEntity = new AIEntity(target->GetEntityID());
        currentEntity->RemoveHealth(1);
    }
}