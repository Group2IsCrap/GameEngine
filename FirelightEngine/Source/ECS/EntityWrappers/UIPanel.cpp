#include "UIPanel.h"

namespace Firelight::ECS
{

	UIPanel::UIPanel() : UIEntity()
	{
		AddComponent<UIContainerComponent>();
	}

	UIContainerComponent* UIPanel::GetContainerComponent()
	{
		return GetComponent<UIContainerComponent>();
	}

}