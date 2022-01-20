#include "GameEntity.h"

namespace Firelight::ECS
{
	GameEntity::GameEntity()
	{
		AddComponent<IdentificationComponent>(new IdentificationComponent());
		AddComponent<TransformComponent>(new TransformComponent());
	}

	IdentificationComponent* GameEntity::GetID()
	{
		return GetComponent<IdentificationComponent>();
	}

	TransformComponent* GameEntity::GetTransform()
	{
		return GetComponent<TransformComponent>();
	}
}