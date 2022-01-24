#include "AnimationSystem.h"

#include "../Components/AnimationComponent.h"
#include "../Components/RenderingComponents.h"
#include "../Components/BasicComponents.h"

namespace Firelight::ECS
{
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

			animatorComponent->currentFrameCount += time.GetGameDeltaTime() * 1000.0;
			if (animatorComponent->currentFrameCount > currentAnimation->m_frameTime)
			{
				animatorComponent->currentFrameIndex++;
				animatorComponent->currentFrameCount = 0.0f;

				if (animatorComponent->currentFrameIndex > currentAnimation->m_frameCount - 1)
				{
					animatorComponent->currentFrameIndex = 0;
				}

				spriteComponent->sourceRect.w = (float)currentAnimation->m_cellWidth;
				spriteComponent->sourceRect.h = (float)currentAnimation->m_cellHeight;
				int xIndex = static_cast<int>(std::floor((animatorComponent->currentFrameIndex / currentAnimation->m_columns)));
				int yIndex = animatorComponent->currentFrameIndex % currentAnimation->m_columns;
				spriteComponent->sourceRect.x = (float)(currentAnimation->m_cellWidth * xIndex);
				spriteComponent->sourceRect.y = (float)(currentAnimation->m_cellHeight * yIndex);
			}
		}
	}
}