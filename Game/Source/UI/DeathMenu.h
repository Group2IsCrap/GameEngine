#pragma once

#include <Source/ECS/EntityWrappers/UICanvas.h>
#include <Source/ECS/EntityWrappers/UIPanel.h>

class DeathMenu : public Firelight::ECS::UIPanel
{
public:
	DeathMenu(Firelight::ECS::Entity* canvas);

	void OpenMenu();
};
