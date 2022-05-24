#include "CombatCalculations.h"
#include <Source/ECS/EntityWrappers/Entity.h>
#include "../Core/CharacterEntity.h"
#include "../Core/AIEntity.h"
#include "../Core/ResourceEntity.h"

#include <Source/Maths/Random.h>

Firelight::ECS::WeaponComponent* CombatCalculations::activeComponent = nullptr;

void CombatCalculations::PlaceSphere(Facing dir, Vec3f point, PlayerEntity* m_player)
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
        std::vector<Firelight::ECS::Entity*> targets = PhysicsHelpers::OverlapCircle(point + offset, activeComponent->Radius, layers[i]);
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
                    if (activeComponent->HarvestType == Firelight::ECS::e_HarvestType(0) && resourceEntity->GetComponent<ResourceComponent>()->resourceID == 0)
                    {
                        resourceEntity->RemoveHealth(activeComponent->HarvestDamage);
                    }
                    else if (activeComponent->HarvestType == Firelight::ECS::e_HarvestType(1) && resourceEntity->GetComponent<ResourceComponent>()->resourceID == 1)
                    {
                        resourceEntity->RemoveHealth(activeComponent->HarvestDamage);
                    }
                    else
                    {
                        resourceEntity->RemoveHealth(1);
                    }
                }
                else if (target->GetComponent<LayerComponent>()->layer == static_cast<int>(GameLayer::Enemy))
                {
                    m_player->GetComponent<AudioComponent>()->soundName = "Attack.wav";
                    m_player->GetComponent<AudioComponent>()->soundPos = Vector3D(m_player->GetTransformComponent()->GetPosition().x, m_player->GetTransformComponent()->GetPosition().y, m_player->GetTransformComponent()->GetPosition().z);

                    m_player->PlayAudioClip();

                    AIEntity* aiEntity = new AIEntity(target->GetEntityID());
                    aiEntity->RemoveHealth(activeComponent->Damage);
                }
            }
            break;
        }
    }
}

void CombatCalculations::ChangeWeapon(WeaponComponent* component)
{
    activeComponent = component;
}