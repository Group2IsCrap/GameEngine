#pragma once

#include "../Maths/Vec3.h"

#include "../ECS/Systems/System.h"
#include "../ECS/Components/BasicComponents.h"
#include "../ECS/Components/PhysicsComponents.h"

#include "../Source/Physics/IntersectData.h"

namespace Firelight::Physics
{
	class PhysicsSystem : public Firelight::ECS::System
	{

	public:

		PhysicsSystem();
		~PhysicsSystem() override;

		void Render();
		void Update(const Utils::Time& time) override;
		void PhysicsUpdate(const Utils::Time& time) override;

		void ApplyForces(double fixedDeltaTime);
		void Simulate(double fixedDeltaTime);
		void HandleCollisions();

	private:

		IntersectData Intersects(Firelight::ECS::Entity* entity, Firelight::ECS::Entity* entity2);

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

		template<typename colliderType, typename colliderType2>
		bool IsPair(Firelight::ECS::ColliderComponent* collider, Firelight::ECS::ColliderComponent* collider2)
		{
			colliderType* collider = dynamic_cast<colliderType*>(collider);
			colliderType2* collider2 = dynamic_cast<colliderType2*>(collider2);

			return (collider != nullptr && collider2 != nullptr);
		}

		Firelight::Maths::Vec3f ComputeForce(Firelight::ECS::RigidBodyComponent* rigidbody);

		bool CheckCollision(Firelight::ECS::TransformComponent* transform, Firelight::ECS::BoxColliderComponent* boxCollider, Firelight::ECS::TransformComponent* transform2, Firelight::ECS::BoxColliderComponent* boxCollider2);
		bool CheckCollision(Firelight::ECS::TransformComponent* transform, Firelight::ECS::CircleColliderComponent* circleCollider, Firelight::ECS::TransformComponent* transform2, Firelight::ECS::CircleColliderComponent* circleCollider2);
		bool CheckCollision(Firelight::ECS::TransformComponent* transform, Firelight::ECS::BoxColliderComponent* boxCollider, Firelight::ECS::TransformComponent* transform2, Firelight::ECS::CircleColliderComponent* circleCollider);

	private:
		size_t m_onEarlyRenderSub;
	};
}
