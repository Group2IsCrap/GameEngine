#pragma once

#include <Source/ECS/EntityWrappers/UICanvas.h>
#include <Source/ECS/EntityWrappers/UIPanel.h>
#include <Source/ECS/EntityWrappers/UIButton.h>
#include <Source/Events/Listener.h>

class MainMenuUI : public Firelight::ECS::UIPanel
{
public:
	MainMenuUI(Firelight::ECS::Entity* canvas);

	void PlayFunction();

	void ExitFunction();

private:
	std::vector<Firelight::ECS::UIEntity*> m_UIEntities;
	Firelight::ECS::UIButton* playButton;
};