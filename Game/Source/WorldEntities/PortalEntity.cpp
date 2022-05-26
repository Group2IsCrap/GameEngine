#include "PortalEntity.h"
#include <Source/ECS/Components/PhysicsComponents.h>

PortalEntity::PortalEntity()
{
	AddComponent<Firelight::ECS::AnimationComponent>();
	
	SpriteComponent* spriteComponent = GetSpriteComponent();
	spriteComponent->texture = Firelight::Graphics::AssetManager::Instance().GetTexture("Sprites/Portal/Portal_0000.png");
	spriteComponent->pixelsPerUnit = 50.0f;
	spriteComponent->layer = static_cast<int>(RenderLayer::Player) + 2;

	AudioComponent* audioComponent = AddComponent<AudioComponent>();
	audioComponent->looping = true;
	audioComponent->is3d = true;
	audioComponent->streaming = true;
	audioComponent->channel = "Ambience";
	//audioComponent->soundName = "Portal.wav";

	RigidBodyComponent* rb = AddComponent<RigidBodyComponent>();
	rb->interpolate = false;

	GetStaticComponent()->isStatic = true;

	BoxColliderComponent* boxCollider = dynamic_cast<BoxColliderComponent*>(AddComponent<ColliderComponent>(new BoxColliderComponent()));
	boxCollider->rect = Firelight::Maths::Rectf(0.0f, -2.5f, 4.0f, 1.0f);

	TransformComponent* transformComponent = GetComponent<TransformComponent>();
	audioComponent->soundPos = Vector3D(transformComponent->GetPosition().x, transformComponent->GetPosition().y, transformComponent->GetPosition().z);
	
	m_playEventIndex = Firelight::Events::EventDispatcher::SubscribeFunction<Firelight::Events::InputEvents::GameStartEvent>(std::bind(&PortalEntity::Play, this));
}

void PortalEntity::Play()
{
	this->PlayAudioClip();
	Firelight::ECS::AnimationSystem::Instance()->Play(this, "Portal");
}