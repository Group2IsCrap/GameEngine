#pragma once
#include "UIEntity.h"

namespace Firelight::ECS
{
	class UIPanel : public UIEntity
	{
	public:
		UIPanel();

		UIContainerComponent* GetContainerComponent();
	};
}