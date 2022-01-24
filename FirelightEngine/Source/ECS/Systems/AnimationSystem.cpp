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
	
	void AnimationSystem::Update(const Utils::Time& time)
	{
		if (m_entities.size() <= 0)
		{
			return;
		}

		for (int entityIndex = 0; entityIndex < m_entities.size(); ++entityIndex)
		{
			auto* spriteComponent = m_entities[entityIndex]->GetComponent<SpriteComponent>();
			auto* animatorComponent = m_entities[entityIndex]->GetComponent<AnimationComponent>();

			if (spriteComponent == nullptr || animatorComponent == nullptr)
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

			if (animatorComponent->currentFrameIndex > currentAnimation->m_frameCount)
			{
				if (!currentAnimation->m_loop)
				{
					continue;
				}
			}

			animatorComponent->currentFrameCount += time.GetGameDeltaTime() * 1000.0f;
			if (animatorComponent->currentFrameCount > currentAnimation->m_frameTime)
			{
				animatorComponent->currentFrameIndex++;
				animatorComponent->currentFrameCount = 0.0f;

				if (animatorComponent->currentFrameIndex > currentAnimation->m_frameCount - 1)
				{
					animatorComponent->currentFrameIndex = 0;
				}

				spriteComponent->sourceRect.w = currentAnimation->m_cellWidth;
				spriteComponent->sourceRect.h = currentAnimation->m_cellHeight;
				int xIndex = static_cast<int>(std::floor((animatorComponent->currentFrameIndex / currentAnimation->m_columns)));
				int yIndex = animatorComponent->currentFrameIndex % currentAnimation->m_columns;
				spriteComponent->sourceRect.x = currentAnimation->m_cellWidth * xIndex;
				spriteComponent->sourceRect.y = currentAnimation->m_cellHeight * yIndex;
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

			auto* spriteComponent = entity->GetComponent<SpriteComponent>();
			spriteComponent->sourceRect.w = animatorComponent->currentAnimation->m_cellWidth;
			spriteComponent->sourceRect.h = animatorComponent->currentAnimation->m_cellHeight;
			spriteComponent->sourceRect.x = 0;
			spriteComponent->sourceRect.y = 0;
		}
	}
}
