#pragma once

#include "../Source/ECS/EntityWrappers/Entity.h"

#include <string>

namespace Firelight::Animation
{
	class AnimationManager
	{
	public:
		static void Play(Firelight::ECS::Entity* entity, std::string animationName);
	};
}
