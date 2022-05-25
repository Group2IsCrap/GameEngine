#pragma once

#include <Source/ECS/EntityWrappers/UIPanel.h>
#include <Source/Events/Listener.h>
#include <Source/ECS/EntityWrappers/UIButton.h>

using namespace Firelight::ECS;

class DeathMenu : public Firelight::ECS::UIPanel, Firelight::Events::Listener
{
public:
	DeathMenu(Firelight::ECS::Entity* canvas);

	void HandleEvents(const char* event, void* data) override;

	void OpenMenu(bool opened);
	void Respawn();

private:
	std::vector<UIEntity*> m_UIEntities;
};
