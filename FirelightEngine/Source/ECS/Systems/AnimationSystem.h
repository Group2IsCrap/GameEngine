#pragma once

#include "System.h"
#include "../Source/Animation/Animation.h"

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

		static AnimationSystem* Instance();

		void GetAllAnimations();

		void Play(Entity* entity, std::string animationName);

	private:
		static AnimationSystem* sm_instance;

		std::map<std::string, class Firelight::Animation::Animation*> m_animations;
	};
}
