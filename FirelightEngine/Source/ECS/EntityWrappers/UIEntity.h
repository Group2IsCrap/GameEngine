#pragma once

#include "GameEntity.h"
#include "../Components/RenderingComponents.h"
#include"../Components/UIComponents.h"
namespace Firelight::ECS
{
	class UIEntity : public GameEntity
	{

	public:
		UIEntity();

		PixleSpriteComponent* GetSpriteComponent();

	};
}
