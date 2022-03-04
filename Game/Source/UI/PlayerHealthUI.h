#pragma once

#include <Source/ECS/EntityWrappers/UICanvas.h>
#include <Source/ECS/EntityWrappers/UIPanel.h>
#include <Source/Events/Listener.h>

class PlayerHealthUI : public Firelight::ECS::UIPanel, Firelight::Events::Listener
{
public:
	PlayerHealthUI(Firelight::ECS::Entity* canvas);

	void HandleEvents(const char* event, void* data) override;
};