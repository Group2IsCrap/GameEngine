#pragma once

#include "System.h"

#include <string>
#include <vector>
#include <map>

namespace Firelight::ECS
{
	class AnimationSystem : public System
	{
	public:
		AnimationSystem();
		~AnimationSystem();

		void Update(const Utils::Time& time) override;
	};
}
