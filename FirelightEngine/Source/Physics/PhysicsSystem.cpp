#include "PhysicsSystem.h"

#include "../Source/Maths/Vec2.h"
#include "../Graphics/Data/Texture.h"
#include "../ECS/Components/RenderingComponents.h"
#include "../ECS/Components/BasicComponents.h"
#include "../Graphics/Data/Colour.h"

#include "../Graphics/SpriteBatch.h"
#include "../Graphics/GraphicsHandler.h"
#include "../Graphics/GraphicsEvents.h"
#include "../Events/EventDispatcher.h"

#include "../Graphics/AssetManager.h"

#include "IntersectData.h"

namespace Firelight::Physics
{
	PhysicsSystem::PhysicsSystem()
	{
		AddWhitelistComponent<Firelight::ECS::RigidBodyComponent>();
		AddWhitelistComponent<Firelight::ECS::ColliderComponent>();

		m_onEarlyRenderSub = Firelight::Events::EventDispatcher::SubscribeFunction<Firelight::Events::Graphics::OnEarlyRender>(std::bind(&PhysicsSystem::Render, this));
	}

	PhysicsSystem::~PhysicsSystem()
	{
		Events::EventDispatcher::UnsubscribeFunction<Events::Graphics::OnEarlyRender>(m_onEarlyRenderSub);
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
				Maths::Rectf sourceRect = Maths::Rectf(0.0f, 0.0f, 0.0f, 0.0f);
				if (entity->HasComponent<Firelight::ECS::ColliderComponent, Firelight::ECS::CircleColliderComponent>())
				{
					texture = Firelight::Graphics::AssetManager::Instance().GetTexture("$ENGINE/Textures/Colliders/Circle.png");
					if (texture == nullptr)
					{
						continue;
					}

					Firelight::ECS::CircleColliderComponent* circleCollider = entity->GetComponent<Firelight::ECS::ColliderComponent, Firelight::ECS::CircleColliderComponent>();

					const Maths::Vec2f spriteWorldSize = Maths::Vec2f((float)texture->GetDimensions().x, (float)texture->GetDimensions().y) / 1.0f;

					destRect = Firelight::Maths::Rectf(
						(transformComponent->position.x - (circleCollider->radius) + spriteComponent->drawOffset.x),
						(transformComponent->position.y - (circleCollider->radius) + spriteComponent->drawOffset.y),
						circleCollider->radius + circleCollider->radius, circleCollider->radius + circleCollider->radius);
					sourceRect = Firelight::Maths::Rectf(0.0f, 0.0f, 100.0f, 100.0f);
				}
				else if (entity->HasComponent<Firelight::ECS::ColliderComponent, Firelight::ECS::BoxColliderComponent>())
				{
					texture = Firelight::Graphics::AssetManager::Instance().GetTexture("$ENGINE/Textures/Colliders/Rectangle.png");
					if (texture == nullptr)
					{
						continue;
					}
					Firelight::ECS::BoxColliderComponent* boxCollider = entity->GetComponent<Firelight::ECS::ColliderComponent, Firelight::ECS::BoxColliderComponent>();

					const Maths::Vec2f spriteWorldSize = Maths::Vec2f((float)texture->GetDimensions().x, (float)texture->GetDimensions().y) / 1.0f;

					destRect = Firelight::Maths::Rectf(
						transformComponent->position.x - ((boxCollider->rect.w * 0.5f)) + spriteComponent->drawOffset.x + boxCollider->rect.x,
						transformComponent->position.y - ((boxCollider->rect.h * 0.5f)) + spriteComponent->drawOffset.y + boxCollider->rect.y,
						boxCollider->rect.w, boxCollider->rect.h);
					sourceRect = Firelight::Maths::Rectf(0.0f, 0.0f, 100.0f,100.0f);
				}

				// Layer 65 is not rendering within the engine as we put a cap on it (64 being max) in the editor.
				// It may be worth documenting that all colliders are rendered on layer 65.
				int layerOverride = 65;
				Firelight::Graphics::GraphicsHandler::Instance().GetSpriteBatch()->WorldDraw(destRect, texture, layerOverride, 0.0, Firelight::Graphics::Colours::sc_blue, sourceRect);
			}
		}
	}

	void PhysicsSystem::Update(const Utils::Time& time)
	{

		for (int i = 0; i < m_entities.size(); ++i)
		{
			Firelight::ECS::RigidBodyComponent* rigidBodyComponent = m_entities[i]->GetComponent<Firelight::ECS::RigidBodyComponent>();
			Firelight::ECS::TransformComponent* transformComponent = m_entities[i]->GetComponent<Firelight::ECS::TransformComponent>();

			if (rigidBodyComponent->interpolate)
			{
				float timeVal = rigidBodyComponent->interpolationTime / time.GetPhysicsTimeStep();

				transformComponent->position = Maths::Vec3f::Lerp(rigidBodyComponent->lastPos, rigidBodyComponent->nextPos, timeVal);
				rigidBodyComponent->interpolationTime += time.GetDeltaTime();
			}
		}
	}

	void PhysicsSystem::FixedUpdate(const Utils::Time& time)
	{

		ApplyForces(time.GetPhysicsTimeStep());
		HandleCollisions();
	}

	void PhysicsSystem::ApplyForces(double fixedDeltaTime)
	{
		for (int i = 0; i < m_entities.size(); ++i)
		{
			Firelight::ECS::RigidBodyComponent* rigidBodyComponent = m_entities[i]->GetComponent<Firelight::ECS::RigidBodyComponent>();
			Firelight::ECS::TransformComponent* transformComponent = m_entities[i]->GetComponent<Firelight::ECS::TransformComponent>();
			if (rigidBodyComponent->interpolate)
			{
				rigidBodyComponent->interpolationTime = 0.0f;
				rigidBodyComponent->lastPos = rigidBodyComponent->nextPos;
				rigidBodyComponent->nextPos += rigidBodyComponent->velocity;

			}
			else
			{
				transformComponent->position += rigidBodyComponent->velocity;

			}

			float val = (1 - fixedDeltaTime * rigidBodyComponent->dragCoefficient);

			rigidBodyComponent->velocity *= val < 0 ? 0 : val;
			if (rigidBodyComponent->velocity.Length() < 0.001)
			{
				rigidBodyComponent->velocity = 0;
			}
		}
	}

	void PhysicsSystem::Simulate(double fixedDeltaTime)
	{
		UNREFERENCED_PARAMETER(fixedDeltaTime);
	}

	IntersectData PhysicsSystem::Intersects(Firelight::ECS::Entity* entity, Firelight::ECS::Entity* entity2)
	{
		if (HasColliderPair<Firelight::ECS::CircleColliderComponent, Firelight::ECS::CircleColliderComponent>(entity, entity2))
		{
			Firelight::ECS::CircleColliderComponent* collider = entity->GetComponent<Firelight::ECS::ColliderComponent, Firelight::ECS::CircleColliderComponent>();
			Firelight::ECS::CircleColliderComponent* collider2 = entity2->GetComponent<Firelight::ECS::ColliderComponent, Firelight::ECS::CircleColliderComponent>();

			float radiusDistance = collider->radius + collider2->radius;
			Firelight::Maths::Vec3f direction = entity2->GetComponent<Firelight::ECS::TransformComponent>()->position - entity->GetComponent<Firelight::ECS::TransformComponent>()->position;

			float directionLength = direction.Length();
			direction.Normalise();

			float distance = directionLength - radiusDistance;

			return IntersectData(distance < 0, direction, distance);
		}
		else if (HasColliderPair<Firelight::ECS::BoxColliderComponent, Firelight::ECS::BoxColliderComponent>(entity, entity2))
		{
			Firelight::ECS::TransformComponent* transform = entity->GetComponent< Firelight::ECS::TransformComponent>();
			Firelight::ECS::TransformComponent* transform2 = entity2->GetComponent< Firelight::ECS::TransformComponent>();
			Firelight::ECS::BoxColliderComponent* collider = entity->GetComponent<Firelight::ECS::ColliderComponent, Firelight::ECS::BoxColliderComponent>();
			Firelight::ECS::BoxColliderComponent* collider2 = entity2->GetComponent<Firelight::ECS::ColliderComponent, Firelight::ECS::BoxColliderComponent>();

			Firelight::Maths::Vec3f halfExtents(collider->rect.w / 2.0f, collider->rect.h / 2.0f, 0.0f);
			Firelight::Maths::Vec3f halfExtents2(collider2->rect.w / 2.0f, collider2->rect.h / 2.0f, 0.0f);
								 
			Firelight::Maths::Vec3f minExtents(transform->position.x - halfExtents.x, transform->position.y - halfExtents.y, 0.0f);
			Firelight::Maths::Vec3f maxExtents(transform->position.x + halfExtents.x, transform->position.y + halfExtents.y, 0.0f);
								 
			Firelight::Maths::Vec3f minExtents2(transform2->position.x - halfExtents2.x, transform2->position.y - halfExtents2.y, 0.0f);
			Firelight::Maths::Vec3f maxExtents2(transform2->position.x + halfExtents2.x, transform2->position.y + halfExtents2.y, 0.0f);
								 
			Firelight::Maths::Vec3f dists(minExtents2 - maxExtents);
			Firelight::Maths::Vec3f dists2(minExtents - maxExtents2);

			Firelight::Maths::Vec3f distances = dists.Length() >= dists2.Length() ? dists : dists2;

			float length = distances.Length();
			distances.Normalise();

			return IntersectData(true, distances, length);
		}

		return IntersectData(false, Firelight::Maths::Vec3f(0.0f, 0.0f, 0.0f), 0.0f);
	}

	void PhysicsSystem::HandleCollisions()
	{
		for (int i = 0; i < m_entities.size(); ++i)
		{
			Firelight::ECS::Entity* entity = m_entities[i];
			Firelight::ECS::ColliderComponent* collider = entity->GetComponent<Firelight::ECS::ColliderComponent>();

			if (collider == nullptr)
				continue;

			if (!collider->isEnabled)
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

				if (entity->GetComponent<Firelight::ECS::StaticComponent>()->isStatic && entity2->GetComponent<Firelight::ECS::StaticComponent>()->isStatic)
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
								pos2 += e1toe2 * 0.02f;
							}
							if (!entity->GetComponent<Firelight::ECS::StaticComponent>()->isStatic)
							{
								pos1 -= e1toe2 * 0.02f;
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
								pos2 += e1toe2 * 0.02f;
							}
							if (!entity->GetComponent<Firelight::ECS::StaticComponent>()->isStatic)
							{
								pos1 -= e1toe2 * 0.02f;
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
						bool collision = true;
						do
						{
							auto& pos1 = entity->GetComponent<Firelight::ECS::TransformComponent>()->position;
							auto& pos2 = entity2->GetComponent<Firelight::ECS::TransformComponent>()->position;

							auto e1toe2 = Firelight::Maths::Vec3f::Normalise(pos2 - pos1);

							if (!entity2->GetComponent<Firelight::ECS::StaticComponent>()->isStatic)
							{
								pos2 += e1toe2 * 0.02f;
							}
							if (!entity->GetComponent<Firelight::ECS::StaticComponent>()->isStatic)
							{
								pos1 -= e1toe2 * 0.02f;
							}
							collision = CheckCollision(entity->GetComponent<Firelight::ECS::TransformComponent>(), entity->GetComponent<Firelight::ECS::ColliderComponent, Firelight::ECS::BoxColliderComponent>(),
								entity2->GetComponent<Firelight::ECS::TransformComponent>(), entity2->GetComponent<Firelight::ECS::ColliderComponent, Firelight::ECS::CircleColliderComponent>());
						} while (collision);
					}
				}
				else if (HasColliderPair<Firelight::ECS::CircleColliderComponent, Firelight::ECS::BoxColliderComponent>(entity, entity2))
				{
					if (CheckCollision(entity->GetComponent<Firelight::ECS::TransformComponent>(), entity2->GetComponent<Firelight::ECS::ColliderComponent, Firelight::ECS::BoxColliderComponent>(),
						entity->GetComponent<Firelight::ECS::TransformComponent>(), entity->GetComponent<Firelight::ECS::ColliderComponent, Firelight::ECS::CircleColliderComponent>()))
					{
						bool collision = true;
						do
						{
							auto& pos1 = entity->GetComponent<Firelight::ECS::TransformComponent>()->position;
							auto& pos2 = entity2->GetComponent<Firelight::ECS::TransformComponent>()->position;

							auto e1toe2 = Firelight::Maths::Vec3f::Normalise(pos2 - pos1);

							if (!entity2->GetComponent<Firelight::ECS::StaticComponent>()->isStatic)
							{
								pos2 += e1toe2 * 0.02f;
							}
							if (!entity->GetComponent<Firelight::ECS::StaticComponent>()->isStatic)
							{
								pos1 -= e1toe2 * 0.02f;
							}
							collision = CheckCollision(entity->GetComponent<Firelight::ECS::TransformComponent>(), entity2->GetComponent<Firelight::ECS::ColliderComponent, Firelight::ECS::BoxColliderComponent>(),
								entity->GetComponent<Firelight::ECS::TransformComponent>(), entity->GetComponent<Firelight::ECS::ColliderComponent, Firelight::ECS::CircleColliderComponent>());
						} while (collision);
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
		Firelight::Maths::Vec2f halfExtents(boxCollider->rect.w / 2.0f, boxCollider->rect.h / 2.0f);
		Firelight::Maths::Vec2f halfExtents2(boxCollider2->rect.w / 2.0f, boxCollider2->rect.h / 2.0f);
		Firelight::Maths::Vec2f offset(transform->position.x + boxCollider->rect.x, transform->position.y + boxCollider->rect.y);
		Firelight::Maths::Vec2f offset2(transform2->position.x + boxCollider2->rect.x, transform2->position.y + boxCollider2->rect.y);

		return !(offset.x + halfExtents.x < offset2.x - halfExtents2.x ||
			offset.x - halfExtents.x > offset2.x + halfExtents2.x ||
			offset.y + halfExtents.y < offset2.y - halfExtents2.y ||
			offset.y - halfExtents.y > offset2.y + halfExtents2.y);
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
		Maths::Vec2f circleDistance;
		circleDistance.x = std::abs(transform2->position.x - transform->position.x);
		circleDistance.y = std::abs(transform2->position.y - transform->position.y);

		if (circleDistance.x > (boxCollider->rect.w / 2 + circleCollider->radius) || circleDistance.y > (boxCollider->rect.h / 2 + circleCollider->radius))
		{
			return false;
		}

		if (circleDistance.x <= (boxCollider->rect.w / 2) || circleDistance.y <= (boxCollider->rect.h / 2))
		{
			return true;
		}

		float cornerDistanceSquared = std::pow((circleDistance.x - boxCollider->rect.w / 2), 2) + 
			std::pow((circleDistance.y - boxCollider->rect.h / 2), 2);

		return (cornerDistanceSquared <= (circleCollider->radius * circleCollider->radius));
	}
}
