#pragma once
#include "UIEntity.h"

#include "../Source/Maths/Vec3.h"

namespace Firelight::ECS
{
	class UICanvas : public UIEntity
	{
	public:
		UICanvas();
		UICanvas(std::string name);
		UICanvas(Firelight::Maths::Vec3f dimensions, int layer = 100, bool draw = false, Firelight::ECS::e_AnchorSettings anchorSettings = Firelight::ECS::e_AnchorSettings::Center);

		UICanvasComponent* GetCanvasComponent();
	};
}