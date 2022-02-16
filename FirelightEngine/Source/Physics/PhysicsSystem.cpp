#include "PhysicsSystem.h"

#include "../Source/Maths/Vec2.h"

namespace Firelight::Physics
{
	PhysicsSystem::PhysicsSystem()
	{
		AddWhitelistComponent<Firelight::ECS::ColliderComponent>();
	}
	
	void PhysicsSystem::PhysicsUpdate(const Utils::Time& time)
	{
		UNREFERENCED_PARAMETER(time);
		HandleCollisions();
	}

	void PhysicsSystem::ApplyForces(double fixedDeltaTime)
	{
		UNREFERENCED_PARAMETER(fixedDeltaTime);
	}

	void PhysicsSystem::Simulate(double fixedDeltaTime)
	{
		UNREFERENCED_PARAMETER(fixedDeltaTime);
	}

	void PhysicsSystem::HandleCollisions()
	{
		for (int i = 0; i < m_entities.size(); ++i)
		{
			Firelight::ECS::Entity* entity = m_entities[i];
			Firelight::ECS::ColliderComponent* collider = entity->GetComponent<Firelight::ECS::ColliderComponent>();

			if (collider == nullptr)
				continue;

			if (!collider->isEnabled || entity->GetComponent<Firelight::ECS::StaticComponent>()->isStatic)
			{
				continue;
			}

			for (int j = i+1; j < m_entities.size(); ++j)
			{
				Firelight::ECS::Entity* entity2 = m_entities[j];
				Firelight::ECS::ColliderComponent* collider2 = entity2->GetComponent<Firelight::ECS::ColliderComponent>();
				if (entity == entity2)
				{
					continue;
				}

				if (!collider2->isEnabled)
				{
					continue;
				}


				if (HasColliderPair<Firelight::ECS::BoxColliderComponent, Firelight::ECS::BoxColliderComponent>(entity, entity2))
				{
					if (CheckCollision(entity->GetComponent<Firelight::ECS::TransformComponent>(), entity->GetComponent<Firelight::ECS::ColliderComponent, Firelight::ECS::BoxColliderComponent>(),
						entity2->GetComponent<Firelight::ECS::TransformComponent>(), entity2->GetComponent<Firelight::ECS::ColliderComponent, Firelight::ECS::BoxColliderComponent>()))
					{
						
					}
				}
				else if (HasColliderPair<Firelight::ECS::CircleColliderComponent, Firelight::ECS::CircleColliderComponent>(entity, entity2))
				{
					if (CheckCollision(entity->GetComponent<Firelight::ECS::TransformComponent>(), entity->GetComponent<Firelight::ECS::ColliderComponent, Firelight::ECS::CircleColliderComponent>(),
						entity2->GetComponent<Firelight::ECS::TransformComponent>(), entity2->GetComponent<Firelight::ECS::ColliderComponent, Firelight::ECS::CircleColliderComponent>()))
					{
						bool collision = true;
						do
						{
							auto& pos1 = entity->GetComponent<Firelight::ECS::TransformComponent>()->position;
							auto& pos2 = entity2->GetComponent<Firelight::ECS::TransformComponent>()->position;

							auto e1toe2 = Firelight::Maths::Vec3f::Normalise(pos2 - pos1);

							if (!entity2->GetComponent<Firelight::ECS::StaticComponent>()->isStatic)
							{
								pos2 += e1toe2 * 0.2f;
							}
							if (!entity->GetComponent<Firelight::ECS::StaticComponent>()->isStatic)
							{
								pos1 -= e1toe2 * 0.2f;
							}
							collision = CheckCollision(entity->GetComponent<Firelight::ECS::TransformComponent>(), entity->GetComponent<Firelight::ECS::ColliderComponent, Firelight::ECS::CircleColliderComponent>(),
								entity2->GetComponent<Firelight::ECS::TransformComponent>(), entity2->GetComponent<Firelight::ECS::ColliderComponent, Firelight::ECS::CircleColliderComponent>());
						} while (collision);

						
					}
				}
				else if (HasColliderPair<Firelight::ECS::BoxColliderComponent, Firelight::ECS::CircleColliderComponent>(entity, entity2))
				{
					if (CheckCollision(entity->GetComponent<Firelight::ECS::TransformComponent>(), entity->GetComponent<Firelight::ECS::ColliderComponent, Firelight::ECS::BoxColliderComponent>(),
						entity2->GetComponent<Firelight::ECS::TransformComponent>(), entity2->GetComponent<Firelight::ECS::ColliderComponent, Firelight::ECS::CircleColliderComponent>()))
					{
						
					}
				}
				else if (HasColliderPair<Firelight::ECS::CircleColliderComponent, Firelight::ECS::BoxColliderComponent>(entity, entity2))
				{
					if (CheckCollision(entity->GetComponent<Firelight::ECS::TransformComponent>(), entity2->GetComponent<Firelight::ECS::ColliderComponent, Firelight::ECS::BoxColliderComponent>(),
						entity->GetComponent<Firelight::ECS::TransformComponent>(), entity->GetComponent<Firelight::ECS::ColliderComponent, Firelight::ECS::CircleColliderComponent>()))
					{
						
					}
				}
			}
		}
	}

	Firelight::Maths::Vec3f PhysicsSystem::ComputeForce(Firelight::ECS::RigidBodyComponent* rigidbody)
	{
		UNREFERENCED_PARAMETER(rigidbody);
		return Firelight::Maths::Vec3f();
	}
	
	bool PhysicsSystem::CheckCollision(Firelight::ECS::TransformComponent* transform, Firelight::ECS::BoxColliderComponent* boxCollider, Firelight::ECS::TransformComponent* transform2, Firelight::ECS::BoxColliderComponent* boxCollider2)
	{
		// This is based on code I found. This would create 1 AABB that we could then compare to the other stuff. I got no clue how to use it though lol - Ben
		// Simple AABB is underneath cause why not
		/*
		// Calculate the position of the four corners in world space by applying
		// The world matrix to the four corners in object space (0, 0, width, height)
		Vector2 tl = Vector2.Transform(Vector2.Zero, matrix);
		Vector2 tr = Vector2.Transform(new Vector2(extents.x, 0), matrix);
		Vector2 bl = Vector2.Transform(new Vector2(0, extents.y), matrix);
		Vector2 br = Vector2.Transform(extents, matrix);

		// Find the minimum and maximum "corners" based on the ones above
		float minX = Min(tl.X, Min(tr.X, Min(bl.X, br.X)));
		float maxX = Max(tl.X, Max(tr.X, Max(bl.X, br.X)));
		float minY = Min(tl.Y, Min(tr.Y, Min(bl.Y, br.Y)));
		float maxY = Max(tl.Y, Max(tr.Y, Max(bl.Y, br.Y)));
		Vector2 min = new Vector2(minX, minY);
		Vector2 max = new Vector2(maxX, maxY);

		// And create the AABB
		RectangleF aabb = new RectangleF(min, max - min);
		*/

		bool x = transform->position.x + boxCollider->rect.x >= transform2->position.x + boxCollider2->rect.x &&
			transform2->position.x + boxCollider2->rect.x >= transform->position.x;
		bool y = transform->position.y + boxCollider->rect.y >= transform2->position.y &&
			transform2->position.y + boxCollider2->rect.y >= transform->position.y;

		return (x && y);
	}
	
	bool PhysicsSystem::CheckCollision(Firelight::ECS::TransformComponent* transform, Firelight::ECS::CircleColliderComponent* circleCollider, Firelight::ECS::TransformComponent* transform2, Firelight::ECS::CircleColliderComponent* circleCollider2)
	{
		float radiusSquared = circleCollider->radius + circleCollider2->radius;
		radiusSquared *= radiusSquared;
		float distSquared = Firelight::Maths::Vec3f::DistSquared(transform->position, transform2->position);

		return (distSquared < radiusSquared);
	}
	
	bool PhysicsSystem::CheckCollision(Firelight::ECS::TransformComponent* transform, Firelight::ECS::BoxColliderComponent* boxCollider, Firelight::ECS::TransformComponent* transform2, Firelight::ECS::CircleColliderComponent* circleCollider)
	{
		UNREFERENCED_PARAMETER(transform);
		UNREFERENCED_PARAMETER(boxCollider);
		UNREFERENCED_PARAMETER(transform2);
		UNREFERENCED_PARAMETER(circleCollider);
		return false;
	}
}
