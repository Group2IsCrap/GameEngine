#include "CameraEntity.h"

namespace Firelight::ECS
{
	CameraEntity::CameraEntity() : GameEntity()
	{
		AddComponent<Camera2DComponent>();
		this->GetIDComponent()->name = "Camera";
	}

	Camera2DComponent* CameraEntity::GetCamera2DComponent()
	{
		return GetComponent<Camera2DComponent>();
	}
}
