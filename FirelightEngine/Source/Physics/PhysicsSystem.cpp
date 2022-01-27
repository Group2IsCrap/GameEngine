#include "PhysicsSystem.h"

#include "../Source/Maths/Vec2.h"
#include "../Graphics/Data/Texture.h"
#include "../ECS/Components/RenderingComponents.h"
#include "../ECS/Components/BasicComponents.h"

#include "../Graphics/SpriteBatch.h"
#include "../Graphics/GraphicsHandler.h"
#include "../Graphics/GraphicsEvents.h"
#include "../Events/EventDispatcher.h"

#include "../Graphics/AssetManager.h"

namespace Firelight::Physics
{
	PhysicsSystem::PhysicsSystem()
	{
		AddWhitelistComponent<Firelight::ECS::ColliderComponent>();

		Firelight::Events::EventDispatcher::SubscribeFunction<Firelight::Events::Graphics::OnEarlyRender>(std::bind(&PhysicsSystem::Render, this));
	}

	PhysicsSystem::~PhysicsSystem()
	{
		// TODO : Unsubscribe from event
		//Events::EventDispatcher::UnsubscribeFunction<Events::Graphics::OnEarlyRender>();
	}
	
	void PhysicsSystem::Render()
	{
		for (int i = 0; i < m_entities.size(); ++i)
		{
			Firelight::ECS::Entity* entity = m_entities[i];
			auto* transformComponent = entity->GetComponent<Firelight::ECS::TransformComponent>();
			auto* spriteComponent = entity->GetComponent<Firelight::ECS::SpriteComponent>();
			Firelight::ECS::ColliderComponent* collider = entity->GetComponent<Firelight::ECS::ColliderComponent>();

			if (collider == nullptr)
				continue;

			if (!collider->isEnabled)
			{
				continue;
			}

			if (collider->drawCollider)
			{
				Firelight::Graphics::Texture* texture = nullptr;
				Maths::Rectf destRect = Maths::Rectf(0.0f, 0.0f, 0.0f, 0.0f);
				if (entity->HasComponent<Firelight::ECS::ColliderComponent, Firelight::ECS::CircleColliderComponent>())
				{
					texture = Firelight::Graphics::AssetManager::Instance().GetTexture("$ENGINE/Textures/Colliders/Circle.png");
					if (texture == nullptr)
					{
						continue;
					}

					Firelight::ECS::CircleColliderComponent* circleCollider = entity->GetComponent<Firelight::ECS::ColliderComponent, Firelight::ECS::CircleColliderComponent>();

					const Maths::Vec2f spriteWorldSize = Maths::Vec2f((float)texture->GetDimensions().x, (float)texture->GetDimensions().y) / spriteComponent->pixelsPerUnit;

					destRect = Firelight::Maths::Rectf(
						transformComponent->position.x - spriteWorldSize.x * 0.5f + spriteComponent->drawOffset.x,
						transformComponent->position.y - spriteWorldSize.y * 0.5f + spriteComponent->drawOffset.y,
						circleCollider->radius + circleCollider->radius, circleCollider->radius + circleCollider->radius);
				}
				else if (entity->HasComponent<Firelight::ECS::ColliderComponent, Firelight::ECS::BoxColliderComponent>())
				{
					texture = Firelight::Graphics::AssetManager::Instance().GetTexture("$ENGINE/Textures/Colliders/Rectangle.png");
					if (texture == nullptr)
					{
						continue;
					}
					Firelight::ECS::BoxColliderComponent* boxCollider = entity->GetComponent<Firelight::ECS::ColliderComponent, Firelight::ECS::BoxColliderComponent>();

					const Maths::Vec2f spriteWorldSize = Maths::Vec2f((float)texture->GetDimensions().x, (float)texture->GetDimensions().y) / spriteComponent->pixelsPerUnit;

					destRect = Firelight::Maths::Rectf(
						transformComponent->position.x - spriteWorldSize.x * 0.5f + spriteComponent->drawOffset.x,
						transformComponent->position.y - spriteWorldSize.y * 0.5f + spriteComponent->drawOffset.y,
						boxCollider->rect.w, boxCollider->rect.h);
				}

				// Layer 65 is not rendering within the engine as we put a cap on it (64 being max) in the editor.
				// It may be worth documenting that all colliders are rendered on layer 65.
				int layerOverride = 65;
				Firelight::Graphics::GraphicsHandler::Instance().GetSpriteBatch()->WorldDraw(destRect, texture, layerOverride, 0.0, Firelight::Graphics::Colours::sc_white, spriteComponent->sourceRect);
			}
		}
	}

	void PhysicsSystem::PhysicsUpdate(const Utils::Time& time)
	{
		HandleCollisions();
	}

	void PhysicsSystem::ApplyForces(double fixedDeltaTime)
	{

	}

	void PhysicsSystem::Simulate(double fixedDeltaTime)
	{

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
							collision = CheckCollision(entity->GetComponent<Firelight::ECS::TransformComponent>(), entity->GetComponent<Firelight::ECS::ColliderComponent, Firelight::ECS::BoxColliderComponent>(),
								entity2->GetComponent<Firelight::ECS::TransformComponent>(), entity2->GetComponent<Firelight::ECS::ColliderComponent, Firelight::ECS::BoxColliderComponent>());
						} while (collision);
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
		return Firelight::Maths::Vec3f();
	}
	
	bool PhysicsSystem::CheckCollision(Firelight::ECS::TransformComponent* transform, Firelight::ECS::BoxColliderComponent* boxCollider, Firelight::ECS::TransformComponent* transform2, Firelight::ECS::BoxColliderComponent* boxCollider2)
	{
		return (transform->position.x < transform2->position.x + boxCollider->rect.w &&
			transform->position.x + boxCollider->rect.w > transform2->position.x &&
			transform->position.y < transform2->position.y + boxCollider2->rect.h &&
			transform->position.y + boxCollider->rect.h > boxCollider2->rect.y);
	}
	
	bool PhysicsSystem::CheckCollision(Firelight::ECS::TransformComponent* transform, Firelight::ECS::CircleColliderComponent* circleCollider, Firelight::ECS::TransformComponent* transform2, Firelight::ECS::CircleColliderComponent* circleCollider2)
	{
		int radiusSquared = circleCollider->radius + circleCollider2->radius;
		radiusSquared *= radiusSquared;
		float distSquared = Firelight::Maths::Vec3f::DistSquared(transform->position, transform2->position);

		return (distSquared < radiusSquared);
	}
	
	bool PhysicsSystem::CheckCollision(Firelight::ECS::TransformComponent* transform, Firelight::ECS::BoxColliderComponent* boxCollider, Firelight::ECS::TransformComponent* transform2, Firelight::ECS::CircleColliderComponent* circleCollider)
	{
		return false;
	}
}
