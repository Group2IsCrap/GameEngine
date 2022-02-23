#include "CameraEntity.h"

namespace Firelight::ECS
{
	CameraEntity::CameraEntity() : GameEntity()
	{
		AddComponent<Camera2DComponent>();
	}

	Camera2DComponent* CameraEntity::GetCamera2DComponent()
	{
		return GetComponent<Camera2DComponent>();
	}
}
