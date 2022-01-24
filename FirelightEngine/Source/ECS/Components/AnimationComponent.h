#pragma once

#include "../Source/Animation/Animation.h"
#include "../ECSDefines.h"
#include "../Source/Graphics/Data/Texture.h"
#include "../Source/Maths/Rect.h"

#include <string>
#include <map>

namespace Firelight::ECS
{
	struct AnimationComponent : BaseComponent
	{
		Firelight::Animation::Animation* currentAnimation = nullptr;
		double currentFrameCount = 0;
		int currentFrameIndex = 0;
		bool playOnStart = true;
		bool shouldPlay = false;
		std::map<std::string, Firelight::Animation::Animation> animations;

		void Serialise() override
		{
			return;
		}
	};
}

