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

		void Update(double deltaTime) override;

		static AnimationSystem* Instance();

		void Play(Entity* entity, std::string animationName);

	private:
		static AnimationSystem* sm_instance;
	};
}
