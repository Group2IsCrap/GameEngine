#include "UIPanel.h"

namespace Firelight::ECS
{

	UIPanel::UIPanel() : UIEntity()
	{
		AddComponent<UIContainerComponent>();
		this->GetIDComponent()->name = "UI Panel";
	}


	UIPanel::UIPanel(std::string name) : UIPanel()
	{
		this->GetIDComponent()->name = name;
	}

	UIContainerComponent* UIPanel::GetContainerComponent()
	{
		return GetComponent<UIContainerComponent>();
	}

}