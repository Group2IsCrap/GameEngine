#pragma once


#include "../Source/ECS/Components/BasicComponents.h"
#include "../Source/ECS/Components/PhysicsComponents.h"
#include "../Source/ECS/EntityManager.h"
#include "../Source/ECS/EntityWrappers/Entity.h"
#include "../Source/ECS/EntityWrappers/SpriteEntity.h"
#include "../Source/Maths/Vec2.h"
#include <Source/Graphics/AssetManager.h>

#include <vector>
#include <math.h>

#define PI 3.14159265

using namespace Firelight::ECS;
using namespace Firelight::Maths;

namespace Firelight::Physics
{
	class PhysicsHelpers
	{
	public:

		static inline std::vector<Entity*> OverlapCircle(Vec3f point, float radius, int layer = 0 )
		{
			return OverlapCircle(point, radius, std::vector<int>{ layer });
		}

		/// <summary>
		/// Returns a vector of entities that intersect within a circle at the given point of given radius on corresponding layer.
		/// </summary>
		/// <param name="point">The location of the point</param>
		/// <param name="radius">The radius of the circle check</param>
		/// <param name="layer">The layer of the entities to check for</param>
		/// <returns>Vector of entities</returns>
		static inline std::vector<Entity*> OverlapCircle(Vec3f point, float radius, std::vector<int> layers = { 0 })
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
				bool entityIsOnLayer = false;
				for (auto layer : layers)
				{
					if (entity->GetComponent<LayerComponent>()->layer == layer)
					{
						entityIsOnLayer = true;
						break;
					}
				}
				if (!entityIsOnLayer)
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

						if (cornerDistanceSquared <= (radius * radius))
						{
							entities.push_back(entity);
							break;
						}
					}
				}
			}

			return entities;
		}

		static inline std::vector<Entity*> OverlapCone(Vec3f point, float radius, float directionalAngle, float theta, int layer = 0)
		{
			return OverlapCone(point, radius, directionalAngle, theta, std::vector<int>{ layer });
		}

		/// <summary>
		/// Overlap function for cone.
		/// </summary>
		/// <param name="point">The location of the point</param>
		/// <param name="radius">The radius of the circle check</param>
		/// <param name="directionalAngle">The facing direction</param>
		/// <param name="theta">The angle to add to each side</param>
		/// <param name="layer">The layer of the entities to check for</param>
		/// <returns>Vector of entities</returns>
		static inline std::vector<Entity*> OverlapCone(Vec3f point, float radius, float directionalAngle, float theta, std::vector<int> layers = { 0 })
		{
			std::vector<Entity*> entities;
			std::vector<ECS::EntityID> allEntities = ECS::EntityComponentSystem::Instance()->GetEntities();
			for (auto entityID : allEntities)
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
				bool entityIsOnLayer = false;
				for (auto layer : layers)
				{
					if (entity->GetComponent<LayerComponent>()->layer == layer)
					{
						entityIsOnLayer = true;
						break;
					}
				}
				if (!entityIsOnLayer)
				{
					continue;
				}

				std::vector<Firelight::ECS::ColliderComponent*> colliders = entity->GetComponents<Firelight::ECS::ColliderComponent>();

				for (int k = 0; k < colliders.size(); ++k)
				{
					
					Firelight::ECS::ColliderComponent* collider = colliders[k];

					Firelight::ECS::CircleColliderComponent* circleCollider = dynamic_cast<Firelight::ECS::CircleColliderComponent*>(collider);
					Firelight::ECS::BoxColliderComponent* boxCollider = dynamic_cast<Firelight::ECS::BoxColliderComponent*>(collider);
					float thetaRadPos = (directionalAngle + theta) * (PI / 180);
					float thetaRadNeg;
					if (directionalAngle == 0)
					{
						thetaRadNeg = (360.0f - theta) * (PI / 180);
					}
					else
					{
						thetaRadNeg = (directionalAngle - theta) * (PI / 180);
					}

					if (circleCollider != nullptr)
					{
						TransformComponent* transform = entity->GetComponent<TransformComponent>();
						float radiusSquared = radius + circleCollider->radius;
						radiusSquared *= radiusSquared;
						float distSquared = Vec3f::DistSquared(point, transform->position);

						if (distSquared < radiusSquared)
						{
							float x1 = std::cos(thetaRadPos);
							float y1 = std::sin(thetaRadPos);
							Vec3f vector1 = Vec3f(x1, y1, 0.0f);
							Vec3f normal1 = Vec3f(-vector1.y, vector1.x, 0.0f);

							if (normal1.Dot(transform->position - point) <= 0)
							{
								float x2 = std::cos(thetaRadNeg);
								float y2 = std::sin(thetaRadNeg);
								Vec3f vector2 = Vec3f(x2, y2, 0.0f);
								Vec3f normal2 = Vec3f(-vector2.y, vector2.x, 0.0f);

								if (normal2.Dot(transform->position - point) >= 0)
								{
									entities.push_back(entity);
									break;
								}
							}
						}
					}
					// Secondly, we check the circle against a box collider
					else if (boxCollider != nullptr)
					{
						Maths::Vec2f circleDistance;
						TransformComponent* transform = entity->GetComponent<TransformComponent>();
						circleDistance.x = std::abs(transform->position.x - point.x);
						circleDistance.y = std::abs(transform->position.y - point.y);

						bool isCollide = false;

						std::vector<Vec3f> corners;
						corners.push_back(Vec3f(	transform->position.x - boxCollider->rect.w / 2, transform->position.y - boxCollider->rect.h / 2, 0.0f));
						corners.push_back(Vec3f(transform->position.x + boxCollider->rect.w / 2, transform->position.y - boxCollider->rect.h / 2, 0.0f));
						corners.push_back(Vec3f(transform->position.x - boxCollider->rect.w / 2, transform->position.y + boxCollider->rect.h / 2, 0.0f));
						corners.push_back(Vec3f(transform->position.x + boxCollider->rect.w / 2, transform->position.y + boxCollider->rect.h / 2, 0.0f));
						corners.push_back(Vec3f(transform->position.x, transform->position.y, 0.0f));
						

						if (circleDistance.x > (boxCollider->rect.w / 2 + radius) || circleDistance.y > (boxCollider->rect.h / 2 + radius))
						{
							continue;
						}

						if (circleDistance.x <= (boxCollider->rect.w / 2) || circleDistance.y <= (boxCollider->rect.h / 2))
						{
							//entities.push_back(entity);
							//break;
						}

						for (int i = 0; i < corners.size(); i++)
						{
							float cornerDistanceSquared = (corners[i].x * corners[i].x) + (corners[i].y * corners[i].y);

							if (cornerDistanceSquared <= (radius * radius))
							{
								float x1 = std::cos(thetaRadPos);
								float y1 = std::sin(thetaRadPos);
								Vec3f vector1 = Vec3f(x1, y1, 0.0f);
								Vec3f normal1 = Vec3f(-vector1.y, vector1.x, 0.0f);;

								Vec3f check = corners[i] - point;

								if (normal1.Dot(check)<= 0)
								{
									float x2 = std::cos(thetaRadNeg);
									float y2 = std::sin(thetaRadNeg);
									Vec3f vector2 = Vec3f(x2, y2, 0.0f);
									Vec3f normal2 = Vec3f(-vector2.y, vector2.x, 0.0f);

									Vec3f check2 = corners[i] - point;

									if (normal2.Dot(check2) >= 0)
									{
										//In Cone
										isCollide = true;
										entities.push_back(entity);
										break;
									}
									else
									{
										continue;
									}
								}
								else
								{
									continue;
								}
							}
						}
						if (isCollide)
						{
							break;
						}
					}
				}
			}

			return entities;
		}

	};
}