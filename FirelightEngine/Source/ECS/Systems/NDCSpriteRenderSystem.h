#pragma once
#include "System.h"
namespace Firelight::ECS
{
	class NDCSpriteRenderSystem : public System
	{
	public:
		NDCSpriteRenderSystem();
		~NDCSpriteRenderSystem();
		void Render();

	};

}