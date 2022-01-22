#pragma once

#include "../ECSDefines.h"
#include "../Source/Graphics/Data/Texture.h"
#include "../Source/Maths/Rect.h"

#include <string>
#include <vector>

namespace Firelight::ECS
{
	struct Animation
	{
		std::string animationName;
		Firelight::Graphics::Texture* texture;
		int cellWidth = 24;
		int cellHeight = 24;
		int frameTime = 100;
		bool loop = true;
		int frameCount = 1;
		int rows;
		int columns;
	};

	struct AnimatorComponent : BaseComponent
	{
		Animation* currentAnimation = nullptr;
		float currentFrameCount = 0;
		int currentFrameIndex = 0;
		bool playOnStart = true;
		std::vector<Animation*> animations;

		void Serialise() override
		{
			return;
		}
	};
}

