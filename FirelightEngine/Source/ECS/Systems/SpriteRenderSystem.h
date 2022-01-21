#pragma once

#include "System.h"

namespace Firelight::ECS
{
	class SpriteRenderSystem : public System
	{
	public:
		SpriteRenderSystem();
		~SpriteRenderSystem();

		void Render();
	};
}
