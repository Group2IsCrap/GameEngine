#pragma once

#include <Source/ECS/Components/UIComponents.h>
#include <Source/ECS/EntityWrappers/UIEntity.h>

#include <Source/Events/Listener.h>

#include <string>
#include <vector>

class HealthUI : public Firelight::Events::Listener
{
public:
	HealthUI(Firelight::ECS::Entity* parent);
	~HealthUI();

	void SetHealthAmount(int health);
	void HandleEvents(const char* event, void* data) override;

private:
	std::string m_iconPath;
	Firelight::ECS::UIEntity* m_uiEntity;
	std::vector<Firelight::ECS::UIEntity*> m_healthUIEntities;
	Firelight::ECS::Entity* m_parentUI;
};