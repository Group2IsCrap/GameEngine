#pragma once

#include "../ECS/System.h"
#include "../ECS/Entity.h"
#include "../ECS/Components/PhysicsComponents.h"

namespace Firelight::Physics
{
	class CollisionHandler : public Firelight::ECS::System
	{

	public:
		void FixedUpdate(double fixedDeltaTime);

		bool CheckCollisions();
		bool HandleCollisions();

	private:

		template<typename collider, typename collider2>
		bool HasColliderPair(Firelight::ECS::Entity* entity, Firelight::ECS::Entity* other)
		{
			if (entity->HasComponent<collider>() && other->HasComponent<collider2>())
				return true;

			return false;
		}

		bool CheckCollision(Firelight::ECS::BoxColliderComponent* collider1, Firelight::ECS::BoxColliderComponent* collider2);
		bool CheckCollision(Firelight::ECS::CircleColliderComponent* collider1, Firelight::ECS::CircleColliderComponent* collider2);
		bool CheckCollision(Firelight::ECS::BoxColliderComponent* collider1, Firelight::ECS::CircleColliderComponent* collider2);

	};
}
