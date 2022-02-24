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
				// Skip if required components are not active
				if (!entity->HasComponent<TransformComponent>() || !entity->HasComponent<LayerComponent>() || !entity->HasComponent<ColliderComponent>())
				{
					continue;
				}

				// Skip if the layer is not the same
				if (!entity->GetComponent<LayerComponent>()->layer != layer)
				{
					continue;
				}

				// Check if the entity is within radius of the point (intersects with circle point)
				// First, we check in case of a circle collider
				if (entity->HasComponent<ColliderComponent, CircleColliderComponent>())
				{
					float radiusSquared = radius + entity->GetComponent<ColliderComponent, CircleColliderComponent>()->radius;
					radiusSquared *= radiusSquared;
					float distSquared = Vec3f::DistSquared(point, entity->GetComponent<TransformComponent>()->position);
					if (distSquared < radiusSquared)
					{
						// Add to list
						entities.push_back(entity);
					}
				}
				// Secondly, we check the circle against a box collider
				else if (entity->HasComponent<ColliderComponent, BoxColliderComponent>())
				{

				}
			}

			return entities;
		}

	};
}