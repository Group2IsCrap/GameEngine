#pragma once

#include "ECSDefines.h"
#include <string>

namespace Engine::ECS
{
	struct IdentificationComponent : BaseComponent
	{
		std::string name;
	};

	struct TransformComponent : BaseComponent
	{
		int posX;
		int posY;
	};

	struct PhysicsComponent : BaseComponent
	{
		int velX;
		int velY;
	};
}