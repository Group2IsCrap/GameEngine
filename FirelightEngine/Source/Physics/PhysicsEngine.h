#pragma once

#include "../Maths/Vec3.h"

#include "../ECS/Systems/System.h"
#include "../ECS/Components/BasicComponents.h"
#include "../ECS/Components/PhysicsComponents.h"

namespace Firelight::Physics
{
	class PhysicsEngine : public Firelight::ECS::System
	{

	public:

		static PhysicsEngine* Instance()
		{
			if (sm_Instance == 0)
			{
				sm_Instance = new PhysicsEngine();
			}

			return sm_Instance;
		}

		void FixedUpdate(double fixedDeltaTime);

		void ApplyForces(double fixedDeltaTime);
		void Simulate(double fixedDeltaTime);
		void HandleCollisions();

	private:

		template<typename component>
		bool HasCollider(Firelight::ECS::Entity* entity)
		{
			if (entity->HasComponent<component>())
				return true;

			return false;
		}

		template<typename collider, typename collider2>
		bool HasColliderPair(Firelight::ECS::Entity* entity, Firelight::ECS::Entity* other)
		{
			if (entity->HasComponent<collider>() && other->HasComponent<collider2>())
				return true;

			return false;
		}

		Firelight::Maths::Vec3f ComputeForce(Firelight::ECS::RigidBodyComponent* rigidbody);

		bool CheckCollision(Firelight::ECS::BoxColliderComponent* boxCollider, Firelight::ECS::BoxColliderComponent* boxCollider2);
		bool CheckCollision(Firelight::ECS::CircleColliderComponent* circleCollider, Firelight::ECS::CircleColliderComponent* circleCollider2);
		bool CheckCollision(Firelight::ECS::BoxColliderComponent* boxCollider, Firelight::ECS::CircleColliderComponent* circleCollider);

	private:

		static PhysicsEngine* sm_Instance;
	};
}
