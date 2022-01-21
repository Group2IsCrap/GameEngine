#include "GameEntity.h"

namespace Firelight::ECS
{
	GameEntity::GameEntity()
	{
		AddComponent<IdentificationComponent>(new IdentificationComponent());
		AddComponent<StaticComponent>(new StaticComponent());
		AddComponent<TransformComponent>(new TransformComponent());
	}
	/// <summary>
	/// Wrapper that returns the Identification Component
	/// </summary>
	/// <returns></returns>
	IdentificationComponent* GameEntity::GetID()
	{
		return GetComponent<IdentificationComponent>();
	}

	/// <summary>
	/// Wrapper that returns the StaticComponent Component
	/// </summary>
	/// <returns></returns>
	StaticComponent* GameEntity::IsStatic()
	{
		return GetComponent<StaticComponent>();
	}

	/// <summary>
	/// Wrapper that returns the Transform Component
	/// </summary>
	/// <returns></returns>
	TransformComponent* GameEntity::GetTransform()
	{
		return GetComponent<TransformComponent>();
	}
}