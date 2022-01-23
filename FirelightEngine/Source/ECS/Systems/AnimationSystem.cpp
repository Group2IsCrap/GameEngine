#include "AnimationSystem.h"

#include "../Components/AnimationComponent.h"
#include "../Components/RenderingComponents.h"
#include "../Components/BasicComponents.h"

namespace Firelight::ECS
{
	AnimationSystem* AnimationSystem::sm_instance = nullptr;

	AnimationSystem* AnimationSystem::Instance()
	{
		if (sm_instance == nullptr)
		{
			sm_instance = new AnimationSystem();
		}
		return sm_instance;
	}

	AnimationSystem::AnimationSystem()
	{
		AddWhitelistComponent<Firelight::ECS::AnimationComponent>();
	}
	
	AnimationSystem::~AnimationSystem()
	{

	}
	
	void AnimationSystem::Update(double deltaTime)
	{
		if (m_entities.size() <= 0)
		{
			return;
		}

		for (int entityIndex = 0; entityIndex < m_entities.size(); ++entityIndex)
		{
			auto* transformComponent = m_entities[entityIndex]->GetComponent<TransformComponent>();
			auto* spriteComponent = m_entities[entityIndex]->GetComponent<SpriteComponent>();
			auto* animatorComponent = m_entities[entityIndex]->GetComponent<AnimationComponent>();

			if (transformComponent == nullptr || spriteComponent == nullptr || animatorComponent == nullptr)
			{
				continue;
			}

			if (animatorComponent->currentAnimation == nullptr)
			{
				continue;
			}

			if (!animatorComponent->shouldPlay || animatorComponent->currentAnimation->m_texture == nullptr)
			{
				continue;
			}

			Firelight::Animation::Animation* currentAnimation = animatorComponent->currentAnimation;
			Firelight::Maths::Vec3i textureSize = currentAnimation->m_texture->GetDimensions();
			
			// Consider this being calculated when the component is first added? Animations won't change during runtime
			currentAnimation->m_rows = static_cast<int>(std::floor(textureSize.x / currentAnimation->m_cellWidth));
			currentAnimation->m_columns = static_cast<int>(std::floor(textureSize.y / currentAnimation->m_cellHeight));

			if (animatorComponent->currentFrameIndex > currentAnimation->m_frameCount)
			{
				if (!currentAnimation->m_loop)
				{
					continue;
				}
			}

			animatorComponent->currentFrameCount += deltaTime * currentAnimation->m_frameTime;
			if (animatorComponent->currentFrameCount > 1)
			{
				animatorComponent->currentFrameIndex++;

				if (animatorComponent->currentFrameIndex > currentAnimation->m_frameCount - 1)
				{
					animatorComponent->currentFrameIndex = 0;
				}

				spriteComponent->sourceRect.x = currentAnimation->m_cellWidth * (animatorComponent->currentFrameIndex % currentAnimation->m_columns);
				spriteComponent->sourceRect.y = currentAnimation->m_cellHeight * static_cast<int>(std::floor((animatorComponent->currentFrameIndex / currentAnimation->m_columns)));
			}
		}
	}
	
	void AnimationSystem::Play(Entity* entity, std::string animationName)
	{
		Firelight::ECS::AnimationComponent* animatorComponent = entity->GetComponent<Firelight::ECS::AnimationComponent>();
		if (animatorComponent == nullptr)
		{
			return;
		}

		if (animatorComponent->animations.size() <= 0)
		{
			return;
		}

		if (animatorComponent->animations.contains(animationName))
		{
			animatorComponent->currentAnimation = &animatorComponent->animations[animationName];
			animatorComponent->currentFrameCount = 0.0f;
			animatorComponent->currentFrameIndex = 0;
			animatorComponent->shouldPlay = true;
		}
	}
}
