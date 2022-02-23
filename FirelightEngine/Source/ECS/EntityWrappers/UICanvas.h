#pragma once
#include "UIEntity.h"

namespace Firelight::ECS
{
	class UICanvas : public UIEntity
	{
	public:
		UICanvas();

		UICanvasComponent* GetCanvasComponent();
	};
}