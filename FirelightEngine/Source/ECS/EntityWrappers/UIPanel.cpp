#include "UIPanel.h"

namespace Firelight::ECS
{

	UIPanel::UIPanel()
	{
		AddComponent<UIContainerComponent>();
	}

	UIContainerComponent* UIPanel::GetContainerComponent()
	{
		return GetComponent<UIContainerComponent>();
	}

}