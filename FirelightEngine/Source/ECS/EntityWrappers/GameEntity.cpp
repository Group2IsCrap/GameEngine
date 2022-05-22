#include "GameEntity.h"

namespace Firelight::ECS
{
	GameEntity::GameEntity() : Entity()
	{
		AddComponent<IdentificationComponent>()->name = "Game Entity";
		AddComponent<StaticComponent>();
		AddComponent<LayerComponent>();
		AddComponent<TransformComponent>();
	}

	GameEntity::GameEntity(std::string name) : GameEntity()
	{
		this->GetIDComponent()->name = name;
	}

	GameEntity::GameEntity(Firelight::ECS::EntityID entityID) : Entity(entityID)
	{

	}

	GameEntity::GameEntity(bool isTemplate, Firelight::ECS::EntityID entityID) : Entity(isTemplate, entityID)
	{

	}

	/// <summary>
	/// Wrapper that returns the Identification Component
	/// </summary>
	/// <returns></returns>
	IdentificationComponent* GameEntity::GetIDComponent()
	{
		return GetComponent<IdentificationComponent>();
	}

	/// <summary>
	/// Wrapper that returns the StaticComponent Component
	/// </summary>
	/// <returns></returns>
	StaticComponent* GameEntity::GetStaticComponent()
	{
		return GetComponent<StaticComponent>();
	}

	/// <summary>
	/// Wrapper that returns the Layer Component
	/// </summary>
	/// <returns></returns>
	LayerComponent* GameEntity::GetLayerComponent()
	{
		return GetComponent<LayerComponent>();
	}

	/// <summary>
	/// Wrapper that returns the Transform Component
	/// </summary>
	/// <returns></returns>
	TransformComponent* GameEntity::GetTransformComponent()
	{
		return GetComponent<TransformComponent>();
	}
}