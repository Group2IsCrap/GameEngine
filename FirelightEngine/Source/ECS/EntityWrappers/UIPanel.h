#pragma once
#include "UIEntity.h"

namespace Firelight::ECS
{
	class UIPanel : public UIEntity
	{
	public:
		UIPanel();

		UIPanel(std::string name);

		UIContainerComponent* GetContainerComponent();
	};
}