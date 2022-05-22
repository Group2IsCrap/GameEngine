#pragma once

#include "GameEntityTemplate.h"

namespace Firelight::ECS
{
	class SpriteEntityTemplate : public GameEntityTemplate
	{
	public:
		SpriteEntityTemplate();
		SpriteEntityTemplate(std::string name);
	};
}
