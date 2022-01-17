#include "GameEntity.h"
namespace Firelight::ECS
{
	GameEntity::GameEntity()
	{
		this->AddComponent<IdentificationComponent>(new IdentificationComponent());
		this->AddComponent<TransformComponent>(new TransformComponent());
	}
	IdentificationComponent* GameEntity::GetID()
	{
		return this->GetComponent<IdentificationComponent>();
	}
	TransformComponent* GameEntity::GetTransform()
	{
		return this->GetComponent<TransformComponent>();
	}
}