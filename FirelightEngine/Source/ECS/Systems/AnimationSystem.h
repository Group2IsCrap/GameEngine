#pragma once

#include "System.h"

#include <string>
#include <vector>

namespace Firelight::ECS
{
	class AnimationSystem : public System
	{
	public:
		AnimationSystem();
		~AnimationSystem();

		void Update(double deltaTime) override;

		void Play(Entity* entity, std::string animationName);
	};
}
