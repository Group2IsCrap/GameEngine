#include "AnimationSystem.h"

#include "../Components/AnimatorComponent.h"
#include "../Components/RenderingComponents.h"
#include "../Components/BasicComponents.h"

namespace Firelight::ECS
{
	AnimationSystem::AnimationSystem()
	{
		AddWhitelistComponent<AnimatorComponent>();
	}
	
	AnimationSystem::~AnimationSystem()
	{

	}
	
	void AnimationSystem::Update(double deltaTime)
	{
		for (int entityIndex = 0; entityIndex < m_entities.size(); ++entityIndex)
		{
			auto* transformComponent = EntityComponentSystem::Instance()->GetComponent<TransformComponent>(m_entities[entityIndex]);
			auto* spriteComponent = EntityComponentSystem::Instance()->GetComponent<SpriteComponent>(m_entities[entityIndex]);
			auto* animatorComponent = EntityComponentSystem::Instance()->GetComponent<AnimatorComponent>(m_entities[entityIndex]);

			if (transformComponent == nullptr || spriteComponent == nullptr || animatorComponent == nullptr)
			{
				continue;
			}
			
			if (animatorComponent->texture == nullptr)
			{
				continue;
			}


		}
	}
}
