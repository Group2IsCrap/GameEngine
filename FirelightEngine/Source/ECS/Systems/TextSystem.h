#pragma once

#include "System.h"

namespace Firelight::ECS
{
	class TextSystem : public System
	{
	public:
		TextSystem();
		~TextSystem();

		void Render();
	};
}
