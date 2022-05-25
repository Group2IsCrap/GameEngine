#include "PortalEntity.h"

PortalEntity::PortalEntity()
{
	
	AddComponent<TransformComponent>();
	sprite = new SpriteComponent();
	transformComponent = GetComponent<TransformComponent>();
	transformComponent->SetPosition(Firelight::Maths::Vec3f(0.0f, 0.0f, 0.0f));
	AddComponent<SpriteComponent>(sprite);
	animationComponent = new AnimationComponent();
	AddComponent<Firelight::ECS::AnimationComponent>(animationComponent);
	audioComponent = new AudioComponent();
	AddComponent<AudioComponent>(audioComponent);

	//GetComponent<RigidBodyComponent>()->nextPos = Firelight::Maths::Vec3f(0.0f, 0.0f, 0.0f);
	sprite->texture = Firelight::Graphics::AssetManager::Instance().GetTexture("Sprites/Portal/Portal_0000.png");
	sprite->pixelsPerUnit = 50.0f;
	sprite->layer = static_cast<int>(RenderLayer::Player) - 2;
	audioComponent->looping = true;
	audioComponent->is3d = false;
	audioComponent->streaming = true;
	audioComponent->channel = "Ambience";
	//audioComponent->soundName = "Portal.wav";
	audioComponent->soundPos = Vector3D(transformComponent->GetPosition().x, transformComponent->GetPosition().y, transformComponent->GetPosition().z);
	m_playEventIndex = Firelight::Events::EventDispatcher::SubscribeFunction<Firelight::Events::InputEvents::GameStartEvent>(std::bind(&PortalEntity::Play, this));
}

void PortalEntity::Play()
{
	this->PlayAudioClip();
	Firelight::ECS::AnimationSystem::Instance()->Play(this, "Portal");
}