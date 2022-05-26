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
#include "../KeyBinder.h"
#include "../Engine.h"

namespace Firelight::Physics
{
	PhysicsSystem::PhysicsSystem()
	{
		AddWhitelistComponent<Firelight::ECS::RigidBodyComponent>();
		AddWhitelistComponent<Firelight::ECS::ColliderComponent>();
		AddWhitelistComponent<Firelight::ECS::StaticComponent>();

		KeyBinder* keyBinder = &Engine::Instance().GetKeyBinder();
		keyBinder->BindKeyboardActionEvent(Firelight::Events::DrawCollidersEvent::sm_descriptor, Keys::KEY_FUNCTION_1, KeyEventType::KeyPressSingle);
		m_drawCollidersIndex = Firelight::Events::EventDispatcher::SubscribeFunction<Firelight::Events::DrawCollidersEvent>(std::bind(&PhysicsSystem::ToggleDrawColliders, this));

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
			//Firelight::ECS::ColliderComponent* collider = entity->GetComponent<Firelight::ECS::ColliderComponent>();
			std::vector<Firelight::ECS::ColliderComponent*> colliders = entity->GetComponents<Firelight::ECS::ColliderComponent>();

			for (int j = 0; j < colliders.size(); ++j)
			{
				Firelight::ECS::ColliderComponent* collider = colliders[j];
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
					Firelight::ECS::CircleColliderComponent* circleCollider = dynamic_cast<Firelight::ECS::CircleColliderComponent*>(collider);
					Firelight::ECS::BoxColliderComponent* boxCollider = dynamic_cast<Firelight::ECS::BoxColliderComponent*>(collider);
					if (circleCollider != nullptr)
					{
						texture = Firelight::Graphics::AssetManager::Instance().GetTexture("$ENGINE/Textures/Colliders/Circle.png");
						if (texture == nullptr)
						{
							continue;
						}

						const Maths::Vec2f spriteWorldSize = Maths::Vec2f((float)texture->GetDimensions().x, (float)texture->GetDimensions().y) / 1.0f;
						Maths::Vec3f position = transformComponent->GetPosition();
						destRect = Firelight::Maths::Rectf(
							(position.x - circleCollider->radius + circleCollider->offset.x + spriteComponent->drawOffset.x),
							(position.y - circleCollider->radius + circleCollider->offset.y + spriteComponent->drawOffset.y),
							circleCollider->radius + circleCollider->radius,
							circleCollider->radius + circleCollider->radius);
						sourceRect = Firelight::Maths::Rectf(0.0f, 0.0f, 100.0f, 100.0f);
					}
					else if (boxCollider != nullptr)
					{
						texture = Firelight::Graphics::AssetManager::Instance().GetTexture("$ENGINE/Textures/Colliders/Rectangle.png");
						if (texture == nullptr)
						{
							continue;
						}

						const Maths::Vec2f spriteWorldSize = Maths::Vec2f((float)texture->GetDimensions().x, (float)texture->GetDimensions().y) / 1.0f;

						Maths::Vec3f position = transformComponent->GetPosition();
						destRect = Firelight::Maths::Rectf(
							position.x - ((boxCollider->rect.w * 0.5f)) + spriteComponent->drawOffset.x + boxCollider->rect.x,
							position.y - ((boxCollider->rect.h * 0.5f)) + spriteComponent->drawOffset.y + boxCollider->rect.y,
							boxCollider->rect.w, boxCollider->rect.h);
						sourceRect = Firelight::Maths::Rectf(0.0f, 0.0f, 100.0f, 100.0f);
					}

					// Layer 1000 is not rendering within the engine as we put a cap on it (64 being max) in the editor.
					// It may be worth documenting that all colliders are rendered on layer 1000.
					int layerOverride = 1000;
					Firelight::Graphics::GraphicsHandler::Instance().GetSpriteBatch()->WorldDraw(destRect, texture, layerOverride, 0.0, Firelight::Graphics::Colours::sc_blue, sourceRect);
				}
			}			
		}
	}

	void PhysicsSystem::Update(const Utils::Time& time, const bool& isPaused)
	{
		if (!isPaused)
		{
			for (int i = 0; i < m_entities.size(); ++i)
			{
				Firelight::ECS::RigidBodyComponent* rigidBodyComponent = m_entities[i]->GetComponent<Firelight::ECS::RigidBodyComponent>();
				Firelight::ECS::TransformComponent* transformComponent = m_entities[i]->GetComponent<Firelight::ECS::TransformComponent>();

				if (rigidBodyComponent->interpolate)
				{
					float timeVal = rigidBodyComponent->interpolationTime / time.GetPhysicsTimeStep();

					transformComponent->SetPosition(Maths::Vec3f::Lerp(rigidBodyComponent->lastPos, rigidBodyComponent->nextPos, timeVal));
					rigidBodyComponent->interpolationTime += time.GetDeltaTime();
				}
			}
		}
	}

	void PhysicsSystem::FixedUpdate(const Utils::Time& time, const bool& isPaused)
	{
		if (!isPaused)
		{
			ApplyForces(time.GetPhysicsTimeStep());
			HandleCollisions();
		}
	}

	void PhysicsSystem::ApplyForces(double fixedDeltaTime)
	{
		for (int i = 0; i < m_entities.size(); ++i)
		{
			Firelight::ECS::RigidBodyComponent* rigidBodyComponent = m_entities[i]->GetComponent<Firelight::ECS::RigidBodyComponent>();
			Firelight::ECS::TransformComponent* transformComponent = m_entities[i]->GetComponent<Firelight::ECS::TransformComponent>();
			rigidBodyComponent->lastPos = rigidBodyComponent->nextPos;
			if (rigidBodyComponent->interpolate)
			{
				rigidBodyComponent->interpolationTime = 0.0f;
				rigidBodyComponent->nextPos += rigidBodyComponent->velocity;
			}
			else
			{
				Maths::Vec3f position = transformComponent->GetPosition();
				transformComponent->SetPosition(transformComponent->GetPosition() + rigidBodyComponent->velocity);
				rigidBodyComponent->nextPos = transformComponent->GetPosition();
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
			Firelight::Maths::Vec3f direction = entity2->GetComponent<Firelight::ECS::TransformComponent>()->GetPosition() - entity->GetComponent<Firelight::ECS::TransformComponent>()->GetPosition();

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
			Maths::Vec3f position = transform->GetPosition();
			Maths::Vec3f position2 = transform2->GetPosition();

			Firelight::Maths::Vec3f halfExtents(collider->rect.w / 2.0f, collider->rect.h / 2.0f, 0.0f);
			Firelight::Maths::Vec3f halfExtents2(collider2->rect.w / 2.0f, collider2->rect.h / 2.0f, 0.0f);
								 
			Firelight::Maths::Vec3f minExtents(position.x - halfExtents.x, position.y - halfExtents.y, 0.0f);
			Firelight::Maths::Vec3f maxExtents(position.x + halfExtents.x, position.y + halfExtents.y, 0.0f);
								 
			Firelight::Maths::Vec3f minExtents2(position2.x - halfExtents2.x, position2.y - halfExtents2.y, 0.0f);
			Firelight::Maths::Vec3f maxExtents2(position2.x + halfExtents2.x, position2.y + halfExtents2.y, 0.0f);
								 
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
			Firelight::ECS::TransformComponent* entityTransform = entity->GetComponent<Firelight::ECS::TransformComponent>();
			Firelight::ECS::RigidBodyComponent* entityRigidBody = entity->GetComponent<Firelight::ECS::RigidBodyComponent>();
			bool entityStatic = entity->GetComponent<Firelight::ECS::StaticComponent>()->isStatic;

			std::vector<Firelight::ECS::ColliderComponent*> colliders = entity->GetComponents<Firelight::ECS::ColliderComponent>();

			for (int k = 0; k < colliders.size(); ++k)
			{
				Firelight::ECS::ColliderComponent* collider = colliders[k];

				if (!collider->isEnabled || collider->isTrigger)
				{
					continue;
				}

				Firelight::ECS::CircleColliderComponent* circleCollider = dynamic_cast<Firelight::ECS::CircleColliderComponent*>(collider);
				Firelight::ECS::BoxColliderComponent* boxCollider = dynamic_cast<Firelight::ECS::BoxColliderComponent*>(collider);

				for (int j = i + 1; j < m_entities.size(); ++j)
				{
					Firelight::ECS::Entity* entity2 = m_entities[j];

					if (entity == entity2)
					{
						continue;
					}

					Firelight::ECS::TransformComponent* entity2Transform = entity2->GetComponent<Firelight::ECS::TransformComponent>();
					Firelight::ECS::RigidBodyComponent* entity2RigidBody = entity2->GetComponent<Firelight::ECS::RigidBodyComponent>();
					bool entity2Static = entity2->GetComponent<Firelight::ECS::StaticComponent>()->isStatic;

					if (entityStatic && entity2Static)
					{
						continue;
					}

					std::vector<Firelight::ECS::ColliderComponent*> colliders2 = entity2->GetComponents<Firelight::ECS::ColliderComponent>();
					for (int l = 0; l < colliders2.size(); ++l)
					{
						Firelight::ECS::ColliderComponent* collider2 = colliders2[l];

						if (!collider2->isEnabled || collider2->isTrigger)
						{
							continue;
						}

						Firelight::ECS::CircleColliderComponent* circleCollider2 = dynamic_cast<Firelight::ECS::CircleColliderComponent*>(collider2);
						Firelight::ECS::BoxColliderComponent* boxCollider2 = dynamic_cast<Firelight::ECS::BoxColliderComponent*>(collider2);

						if (boxCollider != nullptr && boxCollider2 != nullptr)
						{
							if (CheckCollision(entityRigidBody, boxCollider, entity2RigidBody, boxCollider2))
							{
								bool collision = true;
								do
								{
									Maths::Vec3f& pos1 = entityRigidBody->nextPos;
									Maths::Vec3f& pos2 = entity2RigidBody->nextPos;

									Maths::Vec3f e1toe2 = Firelight::Maths::Vec3f::Normalise((pos2 + Maths::Vec3f(boxCollider2->rect.x, boxCollider2->rect.y, 0.0f)) - (pos1 + Maths::Vec3f(boxCollider->rect.x, boxCollider->rect.y, 0.0f)));

									if (!entity2Static)
									{
										pos2 += e1toe2 * 0.02f;
									}
									if (!entityStatic)
									{
										pos1 -= e1toe2 * 0.02f;
									}
									collision = CheckCollision(entityRigidBody, boxCollider, entity2RigidBody, boxCollider2);
								} while (collision);
							}
						}
						if (circleCollider != nullptr && circleCollider2 != nullptr)
						{
							if (CheckCollision(entityRigidBody, circleCollider, entity2RigidBody, circleCollider2))
							{

								bool collision = true;
								do
								{
									Maths::Vec3f& pos1 = entityRigidBody->nextPos;
									Maths::Vec3f& pos2 = entity2RigidBody->nextPos;

									Maths::Vec3f e1toe2 = Firelight::Maths::Vec3f::Normalise((pos2 + Maths::Vec3f(circleCollider2->offset.x, circleCollider2->offset.y, 0.0f)) - (pos1 + Maths::Vec3f(circleCollider->offset.x, circleCollider->offset.y, 0.0f)));

									if (!entity2Static)
									{
										pos2 += e1toe2 * 0.02f;
									}
									if (!entityStatic)
									{
										pos1 -= e1toe2 * 0.02f;
									}
									collision = CheckCollision(entityRigidBody, circleCollider, entity2RigidBody, circleCollider2);
								} while (collision);


							}
						}
						if (boxCollider != nullptr && circleCollider2 != nullptr)
						{
							if (CheckCollision(entityRigidBody, boxCollider, entity2RigidBody, circleCollider2))
							{
								bool collision = true;
								do
								{
									Maths::Vec3f& pos1 = entityRigidBody->nextPos;
									Maths::Vec3f& pos2 = entity2RigidBody->nextPos;

									Maths::Vec3f e1toe2 = Firelight::Maths::Vec3f::Normalise((pos2 + Maths::Vec3f(circleCollider2->offset.x, circleCollider2->offset.y, 0.0f)) - (pos1 + Maths::Vec3f(boxCollider->rect.x, boxCollider->rect.y, 0.0f)));

									if (!entity2Static)
									{
										pos2 += e1toe2 * 0.02f;
									}
									if (!entityStatic)
									{
										pos1 -= e1toe2 * 0.02f;
									}
									collision = CheckCollision(entityRigidBody, boxCollider, entity2RigidBody, circleCollider2);
								} while (collision);
							}
						}
						if (circleCollider != nullptr && boxCollider2 != nullptr)
						{
							if (CheckCollision(entity2RigidBody, boxCollider2, entityRigidBody, circleCollider))
							{
								bool collision = true;
								do
								{
									Maths::Vec3f& pos1 = entityRigidBody->nextPos;
									Maths::Vec3f& pos2 = entity2RigidBody->nextPos;

									Maths::Vec3f e1toe2 = Firelight::Maths::Vec3f::Normalise((pos2 + Maths::Vec3f(boxCollider2->rect.x, boxCollider2->rect.y, 0.0f)) - (pos1 + Maths::Vec3f(circleCollider->offset.x, circleCollider->offset.y, 0.0f)));

									if (!entity2Static)
									{
										pos2 += e1toe2 * 0.02f;
									}
									if (!entityStatic)
									{
										pos1 -= e1toe2 * 0.02f;
									}
									collision = CheckCollision(entity2RigidBody, boxCollider2, entityRigidBody, circleCollider);
								} while (collision);
							}
						}
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

	bool PhysicsSystem::CheckCollision(Firelight::ECS::RigidBodyComponent* rigidBody, Firelight::ECS::BoxColliderComponent* boxCollider, Firelight::ECS::RigidBodyComponent* rigidBody2, Firelight::ECS::BoxColliderComponent* boxCollider2)
	{
		Firelight::Maths::Vec2f halfExtents(boxCollider->rect.w / 2.0f, boxCollider->rect.h / 2.0f);
		Firelight::Maths::Vec2f halfExtents2(boxCollider2->rect.w / 2.0f, boxCollider2->rect.h / 2.0f);
		Firelight::Maths::Vec2f offset(rigidBody->nextPos.x + boxCollider->rect.x, rigidBody->nextPos.y + boxCollider->rect.y);
		Firelight::Maths::Vec2f offset2(rigidBody2->nextPos.x + boxCollider2->rect.x, rigidBody2->nextPos.y + boxCollider2->rect.y);

		return !(offset.x + halfExtents.x < offset2.x - halfExtents2.x ||
			offset.x - halfExtents.x > offset2.x + halfExtents2.x ||
			offset.y + halfExtents.y < offset2.y - halfExtents2.y ||
			offset.y - halfExtents.y > offset2.y + halfExtents2.y);
	}

	bool PhysicsSystem::CheckCollision(Firelight::ECS::RigidBodyComponent* rigidBody, Firelight::ECS::CircleColliderComponent* circleCollider, Firelight::ECS::RigidBodyComponent* rigidBody2, Firelight::ECS::CircleColliderComponent* circleCollider2)
	{
		float radiusSquared = circleCollider->radius + circleCollider2->radius;
		radiusSquared *= radiusSquared;
		float distSquared = Firelight::Maths::Vec3f::DistSquared(rigidBody->nextPos + Maths::Vec3f(circleCollider->offset.x, circleCollider->offset.y,0.0f), rigidBody2->nextPos + Maths::Vec3f(circleCollider2->offset.x, circleCollider2->offset.y, 0.0f));

		return (distSquared < radiusSquared);
	}
	
	bool PhysicsSystem::CheckCollision(Firelight::ECS::RigidBodyComponent* rigidBody, Firelight::ECS::BoxColliderComponent* boxCollider, Firelight::ECS::RigidBodyComponent* rigidBody2, Firelight::ECS::CircleColliderComponent* circleCollider)
	{
		Maths::Vec2f circleDistance;
		circleDistance.x = abs((rigidBody2->nextPos.x + circleCollider->offset.x) - (rigidBody->nextPos.x + boxCollider->rect.x));
		circleDistance.y = abs((rigidBody2->nextPos.y + circleCollider->offset.y) - (rigidBody->nextPos.y + boxCollider->rect.y));

		if (circleDistance.x > (boxCollider->rect.w / 2 + circleCollider->radius)) { return false; }
		if (circleDistance.y > (boxCollider->rect.h / 2 + circleCollider->radius)) { return false; }

		if (circleDistance.x <= (boxCollider->rect.w / 2)) { return true; }
		if (circleDistance.y <= (boxCollider->rect.h / 2)) { return true; }

		float cornerDistanceSquared = std::pow((circleDistance.x - boxCollider->rect.w / 2), 2) + std::pow((circleDistance.y - boxCollider->rect.h / 2), 2);


		return (cornerDistanceSquared <= (circleCollider->radius * circleCollider->radius));



	}
	
	void PhysicsSystem::ToggleDrawColliders()
	{
		m_drawColliders = !m_drawColliders;

		for (auto* entity : m_entities)
		{
			std::vector<Firelight::ECS::ColliderComponent*> colliderComponents = entity->GetComponents<Firelight::ECS::ColliderComponent>();
			for (auto* collider : colliderComponents)
			{
				collider->drawCollider = m_drawColliders;
			}
		}
	}
}
