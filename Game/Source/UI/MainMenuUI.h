#pragma once

#include <Source/ECS/EntityWrappers/UICanvas.h>
#include <Source/ECS/EntityWrappers/UIPanel.h>
#include <Source/ECS/EntityWrappers/UIButton.h>
#include <Source/Events/Listener.h>

class MainMenuUI : public Firelight::ECS::UIPanel, Firelight::Events::Listener
{
public:
	MainMenuUI(Firelight::ECS::Entity* canvas);

	void HandleEvents(const char* event, void* data) override;

	void PlayFunction();

	void ExitFunction();

private:
	std::vector<Firelight::ECS::UIEntity*> m_UIEntities;
};