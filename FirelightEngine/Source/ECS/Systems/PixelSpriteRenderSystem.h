#pragma once
#include "System.h"
namespace Firelight::ECS
{
	class PixelSpriteRenderSystem : public System
	{
	public:
		PixelSpriteRenderSystem();
		~PixelSpriteRenderSystem();
		void Render();

	};
}
