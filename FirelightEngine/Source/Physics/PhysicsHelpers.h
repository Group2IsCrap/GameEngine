#pragma once


#include "../Source/ECS/Components/BasicComponents.h"
#include "../Source/ECS/Components/PhysicsComponents.h"
#include "../Source/ECS/EntityManager.h"
#include "../Source/ECS/EntityWrappers/Entity.h"
#include "../Source/Maths/Vec2.h"

#include <vector>

using namespace Firelight::ECS;
using namespace Firelight::Maths;

namespace Firelight::Physics
{
	class PhysicsHelpers
	{
	public:

		/// <summary>
		/// Returns a vector of entities that intersect within a circle at the given point of given radius on corresponding layer.
		/// </summary>
		/// <param name="point">The location of the point</param>
		/// <param name="radius">The radius of the circle check</param>
		/// <param name="layer">The layer of the entities to check for</param>
		/// <returns>Vector of entities</returns>
		static inline std::vector<Entity*> OverlapCircle(Vec3f point, float radius, int layer = 0)
		{
			std::vector<Entity*> entities;
			std::vector<ECS::EntityID> allEntities = ECS::EntityComponentSystem::Instance()->GetEntities();
			for(auto entityID : allEntities)
			{
				Entity* entity = new Entity(entityID);

				if (std::find(entities.begin(), entities.end(), entity) != entities.end())
				{
					continue;
				}

				// Skip if required components are not active
				if (!entity->HasComponent<TransformComponent>() || !entity->HasComponent<LayerComponent>() || !entity->HasComponent<ColliderComponent>())
				{
					continue;
				}

				// Skip if the layer is not the same
				if (entity->GetComponent<LayerComponent>()->layer != layer)
				{
					continue;
				}

				std::vector<Firelight::ECS::ColliderComponent*> colliders = entity->GetComponents<Firelight::ECS::ColliderComponent>();

				for (int k = 0; k < colliders.size(); ++k)
				{
					Firelight::ECS::ColliderComponent* collider = colliders[k];

					Firelight::ECS::CircleColliderComponent* circleCollider = dynamic_cast<Firelight::ECS::CircleColliderComponent*>(collider);
					Firelight::ECS::BoxColliderComponent* boxCollider = dynamic_cast<Firelight::ECS::BoxColliderComponent*>(collider);

					if (circleCollider != nullptr)
					{
						float radiusSquared = radius + circleCollider->radius;
						radiusSquared *= radiusSquared;
						float distSquared = Vec3f::DistSquared(point, entity->GetComponent<TransformComponent>()->position);
						if (distSquared < radiusSquared)
						{
							// Add to list
							entities.push_back(entity);
							break;
						}
					}
					// Secondly, we check the circle against a box collider
					else if (boxCollider != nullptr)
					{
						Maths::Vec2f circleDistance;
						TransformComponent* transform = entity->GetComponent<TransformComponent>();
						circleDistance.x = std::abs(transform->position.x - point.x);
						circleDistance.y = std::abs(transform->position.y - point.y);

						if (circleDistance.x > (boxCollider->rect.w / 2 + radius) || circleDistance.y > (boxCollider->rect.h / 2 + radius))
						{
							continue;
						}

						if (circleDistance.x <= (boxCollider->rect.w / 2) || circleDistance.y <= (boxCollider->rect.h / 2))
						{
							entities.push_back(entity);
							break;
						}

						float cornerDistanceSquared = std::pow((circleDistance.x - boxCollider->rect.w / 2), 2) +
							std::pow((circleDistance.y - boxCollider->rect.h / 2), 2);

						if (cornerDistanceSquared <= (radius * radius) == true)
						{
							entities.push_back(entity);
							break;
						}
					}
				}
			}

			return entities;
		}

	};
}