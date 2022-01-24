#include "AnimationManager.h"

#include "../Source/ECS/Components/AnimationComponent.h"
#include "../Source/ECS/Components/RenderingComponents.h"

namespace Firelight::Animation
{
	void AnimationManager::Play(Firelight::ECS::Entity* entity, std::string animationName)
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

			auto* spriteComponent = entity->GetComponent<Firelight::ECS::SpriteComponent>();
			spriteComponent->sourceRect.w = (float)animatorComponent->currentAnimation->m_cellWidth;
			spriteComponent->sourceRect.h = (float)animatorComponent->currentAnimation->m_cellHeight;
			spriteComponent->sourceRect.x = 0.0f;
			spriteComponent->sourceRect.y = 0.0f;
		}
	}
}