#pragma once

#include "../Maths/Vec3.h"

#include "../ECS/Systems/System.h"
#include "../ECS/Components/BasicComponents.h"
#include "../ECS/Components/PhysicsComponents.h"

namespace Firelight::Physics
{
	class PhysicsSystem : public Firelight::ECS::System
	{

	public:

		PhysicsSystem();

		void PhysicsUpdate(const Utils::Time& time) override;

		void ApplyForces(double fixedDeltaTime);
		void Simulate(double fixedDeltaTime);
		void HandleCollisions();

	private:

		template<typename component>
		bool HasCollider(Firelight::ECS::Entity* entity)
		{
			if (entity->HasComponent<Firelight::ECS::ColliderComponent, component>())
				return true;

			return false;
		}

		template<typename collider, typename collider2>
		bool HasColliderPair(Firelight::ECS::Entity* entity, Firelight::ECS::Entity* other)
		{
			if (entity->HasComponent<Firelight::ECS::ColliderComponent, collider>() && other->HasComponent<Firelight::ECS::ColliderComponent, collider2>())
				return true;

			return false;
		}

		Firelight::Maths::Vec3f ComputeForce(Firelight::ECS::RigidBodyComponent* rigidbody);

		bool CheckCollision(Firelight::ECS::TransformComponent* transform, Firelight::ECS::BoxColliderComponent* boxCollider, Firelight::ECS::TransformComponent* transform2, Firelight::ECS::BoxColliderComponent* boxCollider2);
		bool CheckCollision(Firelight::ECS::TransformComponent* transform, Firelight::ECS::CircleColliderComponent* circleCollider, Firelight::ECS::TransformComponent* transform2, Firelight::ECS::CircleColliderComponent* circleCollider2);
		bool CheckCollision(Firelight::ECS::TransformComponent* transform, Firelight::ECS::BoxColliderComponent* boxCollider, Firelight::ECS::TransformComponent* transform2, Firelight::ECS::CircleColliderComponent* circleCollider);
	};
}
