#include "PhysicsEngine.h"

namespace Firelight::Physics
{
	PhysicsEngine* PhysicsEngine::sm_Instance = 0;

	void Firelight::Physics::PhysicsEngine::FixedUpdate(double fixedDeltaTime)
	{
		HandleCollisions();
	}

	void Firelight::Physics::PhysicsEngine::ApplyForces(double fixedDeltaTime)
	{

	}

	void Firelight::Physics::PhysicsEngine::Simulate(double fixedDeltaTime)
	{

	}

	void Firelight::Physics::PhysicsEngine::HandleCollisions()
	{
		for (auto entityID : GetEntities())
		{
			Firelight::ECS::Entity* entity = new Firelight::ECS::Entity(entityID);
			Firelight::ECS::ColliderComponent* collider = entity->GetComponent<Firelight::ECS::ColliderComponent>();

			if (collider == nullptr)
				continue;

			if (!collider->isEnabled) //|| collider->isStatic)
			{
				delete entity;
				continue;
			}

			for (auto entity2ID : GetEntities())
			{
				Firelight::ECS::Entity* entity2 = new Firelight::ECS::Entity(entity2ID);
				Firelight::ECS::ColliderComponent* collider2 = entity->GetComponent<Firelight::ECS::ColliderComponent>();
				if (entity == entity2)
				{
					delete entity2;
					continue;
				}

				if (!collider2->isEnabled)
				{
					delete entity2;
					continue;
				}

				bool collision = false;
				if (HasColliderPair<Firelight::ECS::BoxColliderComponent, Firelight::ECS::BoxColliderComponent>(entity, entity2))
				{
					if (CheckCollision(entity->GetComponent<Firelight::ECS::BoxColliderComponent>(), entity2->GetComponent<Firelight::ECS::BoxColliderComponent>()))
					{
						collision = true;
					}
				}
				else if (HasColliderPair<Firelight::ECS::CircleColliderComponent, Firelight::ECS::CircleColliderComponent>(entity, entity2))
				{
					if (CheckCollision(entity->GetComponent<Firelight::ECS::CircleColliderComponent>(), entity2->GetComponent<Firelight::ECS::CircleColliderComponent>()))
					{
						collision = true;
					}
				}
				else if (HasColliderPair<Firelight::ECS::BoxColliderComponent, Firelight::ECS::CircleColliderComponent>(entity, entity2))
				{
					if (CheckCollision(entity->GetComponent<Firelight::ECS::BoxColliderComponent>(), entity2->GetComponent<Firelight::ECS::CircleColliderComponent>()))
					{
						collision = true;
					}
				}
				else if (HasColliderPair<Firelight::ECS::CircleColliderComponent, Firelight::ECS::BoxColliderComponent>(entity, entity2))
				{
					if (CheckCollision(entity2->GetComponent<Firelight::ECS::BoxColliderComponent>(), entity->GetComponent<Firelight::ECS::CircleColliderComponent>()))
					{
						collision = true;
					}
				}

				if (collision)
				{
					// Respond to collision
				}

				delete entity2;
			}

			delete entity;
		}
	}

	Firelight::Maths::Vec3f Firelight::Physics::PhysicsEngine::ComputeForce(Firelight::ECS::RigidBodyComponent* rigidbody)
	{
		return Firelight::Maths::Vec3f();
	}
	
	bool PhysicsEngine::CheckCollision(Firelight::ECS::BoxColliderComponent* boxCollider, Firelight::ECS::BoxColliderComponent* boxCollider2)
	{
		return false;
	}
	
	bool PhysicsEngine::CheckCollision(Firelight::ECS::CircleColliderComponent* circleCollider, Firelight::ECS::CircleColliderComponent* circleCollider2)
	{
		return false;
	}
	
	bool PhysicsEngine::CheckCollision(Firelight::ECS::BoxColliderComponent* boxCollider, Firelight::ECS::CircleColliderComponent* circleCollider)
	{
		return false;
	}
}
